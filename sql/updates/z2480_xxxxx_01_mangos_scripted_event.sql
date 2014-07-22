ALTER TABLE  `db_version` CHANGE `required_z2478_xxxxx_01_mangos_spell_affect` `required_z2480_xxxxx_01_mangos_scripted_event   ` BIT(1) NULL DEFAULT NULL;

RENAME TABLE  `scripted_event_id` TO  `scripted_event` ;
