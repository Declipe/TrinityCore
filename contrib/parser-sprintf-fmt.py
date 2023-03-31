import pathlib

# tools has been written for move TC_LOG_... with args like %s, %i in fmt::format like {}
# for usage place this file in src/ and run .py

def isTClog(line):
    substring = 'TC_LOG_'
    substring2 = 'FMT_LOG_'
    if substring in line:
        return True
    elif substring2 in line:
        return True
    else :
        return False
        
def isASSERT(line):
    substring = 'ASSERT'
    if substring in line:
        return True
    else :
        return False
        
def isABORTMSG(line):
    substring = 'ABORT_MSG'
    if substring in line:
        return True
    else :
        return False
        
def isSendSysMessage(line):
    substring = 'SendSysMessage'
    if substring in line:
        return True
    else :
        return False
        
def isASSERT(line):
    substring = 'ASSERT'
    if substring in line:
        return True
    else :
        return False
        
def isTCStringFormat(line):
    substring = 'StringFormat('
    if substring in line:
        return True
    else :
        return False
        
def isoutCommand(line):
    substring = 'outCommand('
    if substring in line:
        return True
    else :
        return False

def haveDelimeter(line):
    if ';' in line:
        return True
    else :
        return False

def checkSoloLine(line):
    if isTClog(line):
        line = line.replace("FMT_LOG_", "TC_LOG_");
        return handleCleanup(line), False
    #elif isTCStringFormat(line):
    #    return handleCleanup(line), False
    #elif isASSERT(line):
    #    return handleCleanup(line), False
    #elif isABORTMSG(line):
    #    return handleCleanup(line), False
    #elif isSendSysMessage(line):
    #    return handleCleanup(line), False
    #elif isoutCommand(line):
    #    return handleCleanup(line), False
    else:
        return line, False

def startMultiLine(line):
    if isTClog(line):
        line = line.replace("FMT_LOG_", "TC_LOG_");
        print('Старт мультилайна: \n' + line)
        return handleCleanup(line), True
    #elif isTCStringFormat(line):
    #    print('Старт мультилайна: \n' + line)
    #    return handleCleanup(line), True
    #elif isASSERT(line):
    #    print('Старт мультилайна: \n' + line)
    #    return handleCleanup(line), True
    #elif isABORTMSG(line):
    #    print('Старт мультилайна: \n' + line)
    #    return handleCleanup(line), True
    #elif isSendSysMessage(line):
    #    print('Старт мультилайна: \n' + line)
    #    return handleCleanup(line), True
    #elif isoutCommand(line):
    #    print('Старт мультилайна: \n' + line)
    #    return handleCleanup(line), True
    else :
        return line, False
        
def continueMultiLine(line, existPrevLine):
    if haveDelimeter(line):
        existPrevLine = False;
        print('Продолжение мультилайна: \n' + line)
    return handleCleanup(line), existPrevLine
        
def checkTextLine(line, existPrevLine):
    if existPrevLine:
        return continueMultiLine(line, existPrevLine)
    else :
        if haveDelimeter(line):
            return checkSoloLine(line)
        else :
            return startMultiLine(line)

def handleCleanup(line):
    line = line.replace("%s", "{}");
    line = line.replace("%u", "{}");
    line = line.replace("%hu", "{}");
    line = line.replace("%lu", "{}");
    line = line.replace("%llu", "{}");
    line = line.replace("%02u", "{:02}");
    line = line.replace("%03u", "{:03}");
    line = line.replace("%04u", "{:04}");
    line = line.replace("%05u", "{:05}");
    line = line.replace("%02i", "{:02}");
    line = line.replace("%03i", "{:03}");
    line = line.replace("%04i", "{:04}");
    line = line.replace("%05i", "{:05}");
    line = line.replace("%02d", "{:02}");
    line = line.replace("%03d", "{:03}");
    line = line.replace("%04d", "{:04}");
    line = line.replace("%05d", "{:05}");
    line = line.replace("%d", "{}");
    line = line.replace("%i", "{}");
    line = line.replace("%x", "{:x}");
    line = line.replace("%X", "{:X}");
    line = line.replace("%lx", "{:x}");
    line = line.replace("%lX", "{:X}");
    line = line.replace("%02X", "{:02X}");
    line = line.replace("%08X", "{:08X}");
    line = line.replace("%f", "{}");
    line = line.replace("%.1f", "{0:.1f}");
    line = line.replace("%.2f", "{0:.2f}");
    line = line.replace("%.3f", "{0:.3f}");
    line = line.replace("%.4f", "{0:.4f}");
    line = line.replace("%.5f", "{0:.5f}");
    line = line.replace("%3.1f", "{:3.1f}");
    line = line.replace("%%", "%");
    line = line.replace(".c_str()", "");
    line = line.replace("\" SZFMTD \"", "{}");
    line = line.replace("\" UI64FMTD \"", "{}");
    line = line.replace("\" STRING_VIEW_FMT \"", "{}");
    line = line.replace("STRING_VIEW_FMT_ARG", "");
    return line

def getModifiedfile(name):
    # получим объект файла
    file1 = open(name, "r+")
    
    # проверочная строка, используется, если в строке нет символа ; (окончания функции для TC_LOG...(...);)
    prevLines = False
    result = ''

    while True:
        # считываем строку
        line = file1.readline()
        
        # прерываем цикл, если строка пустая
        if not line:
            break
        
        # проверяем строку
        line, prevLines = checkTextLine(line, prevLines)
        result += line
        
    # закрываем файл
    file1.close
    return result

def updModifiedfile(name, text):
    file = open(name, "w")
    file.write(text)
    file.close()

def handlefile(name):
    # получим объект файла
    newtext = getModifiedfile(name)
    updModifiedfile(name, newtext)

# определение пути
p = pathlib.Path('.')
for i in p.glob('**/*'):
    fname = i.absolute()
    print(fname)
    if '.cpp' in i.name:
        handlefile(fname)
    if '.h' in i.name:
        handlefile(fname)