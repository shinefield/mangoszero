ALTER TABLE `db_version` CHANGE COLUMN `required_z2483_xxxxx_01_mangos_command` `required_z2483_xxxxx_02_mangos_string`bit;

DELETE FROM `mangos_string`
WHERE `entry` IN (
    1171,
    1172,
    1173,
    1174,
    1175,
    1176,
    1177,
    1178,
    1179,
    1180,
    1181,
    1182,
    1183,
    1184,
    1185,
    1186,
    1187,
    1188,
    1189,
    1190,
    1191
);
