ALTER TABLE trinity_string ADD content_enGB mediumtext NULL;
ALTER TABLE trinity_string CHANGE content_enGB content_enGB mediumtext NULL AFTER content_default;
ALTER TABLE trinity_string ADD content_enCN mediumtext NULL;
ALTER TABLE trinity_string CHANGE content_enCN content_enCN mediumtext NULL AFTER content_loc3;
ALTER TABLE trinity_string ADD content_enTW mediumtext NULL;
ALTER TABLE trinity_string CHANGE content_enTW content_enTW mediumtext NULL AFTER content_loc4;
ALTER TABLE trinity_string ADD content_ptPT mediumtext NULL;
ALTER TABLE trinity_string ADD content_ptBR mediumtext NULL;
ALTER TABLE trinity_string ADD content_itIT mediumtext NULL;