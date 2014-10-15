ALTER TABLE `db_version` CHANGE COLUMN `required_z2482_s2128_12654_01_mangos_creature_template_power` `required_z2483_xxxxx_01_mangos_command` bit;

DELETE FROM `command`
WHERE `name` IN (
    'ahbot items amount',
    'ahbot items amount blue',
    'ahbot items amount green',
    'ahbot items amount grey',
    'ahbot items amount orange',
    'ahbot items amount purple',
    'ahbot items amount white',
    'ahbot items amount yellow',
    'ahbot items ratio',
    'ahbot items ratio alliance',
    'ahbot items ratio horde',
    'ahbot items ratio neutral',
    'ahbot rebuild',
    'ahbot reload',
    'ahbot status'
);
