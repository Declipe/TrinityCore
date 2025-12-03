ALTER TABLE trinity_string2 ADD content_enGB mediumtext NULL;
ALTER TABLE trinity_string2 CHANGE content_enGB content_enGB mediumtext NULL AFTER content_default;
ALTER TABLE trinity_string2 ADD content_enCN mediumtext NULL;
ALTER TABLE trinity_string2 CHANGE content_enCN content_enCN mediumtext NULL AFTER content_deDE;
ALTER TABLE trinity_string2 ADD content_enTW mediumtext NULL;
ALTER TABLE trinity_string2 CHANGE content_enTW content_enTW mediumtext NULL AFTER content_zhCN;
ALTER TABLE trinity_string2 ADD content_ptPT mediumtext NULL;
ALTER TABLE trinity_string2 ADD content_ptBR mediumtext NULL;
ALTER TABLE trinity_string2 ADD content_itIT mediumtext NULL;
