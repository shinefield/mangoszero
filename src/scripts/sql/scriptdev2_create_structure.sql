
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
DROP TABLE IF EXISTS `custom_texts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `custom_texts` (
  `entry` mediumint(8) NOT NULL,
  `content_default` text NOT NULL,
  `content_loc1` text,
  `content_loc2` text,
  `content_loc3` text,
  `content_loc4` text,
  `content_loc5` text,
  `content_loc6` text,
  `content_loc7` text,
  `content_loc8` text,
  `sound` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `language` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `emote` smallint(5) unsigned NOT NULL DEFAULT '0',
  `comment` text,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Custom Texts';
/*!40101 SET character_set_client = @saved_cs_client */;

LOCK TABLES `custom_texts` WRITE;
/*!40000 ALTER TABLE `custom_texts` DISABLE KEYS */;
/*!40000 ALTER TABLE `custom_texts` ENABLE KEYS */;
UNLOCK TABLES;
DROP TABLE IF EXISTS `gossip_texts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gossip_texts` (
  `entry` mediumint(8) NOT NULL,
  `content_default` text NOT NULL,
  `content_loc1` text,
  `content_loc2` text,
  `content_loc3` text,
  `content_loc4` text,
  `content_loc5` text,
  `content_loc6` text,
  `content_loc7` text,
  `content_loc8` text,
  `comment` text,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Gossip Texts';
/*!40101 SET character_set_client = @saved_cs_client */;

LOCK TABLES `gossip_texts` WRITE;
/*!40000 ALTER TABLE `gossip_texts` DISABLE KEYS */;
INSERT INTO `gossip_texts` VALUES (-3000000,'[PH] SD2 unknown text',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'GOSSIP_ID_UNKNOWN_TEXT');
/*!40000 ALTER TABLE `gossip_texts` ENABLE KEYS */;
UNLOCK TABLES;
DROP TABLE IF EXISTS `script_texts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `script_texts` (
  `entry` mediumint(8) NOT NULL,
  `content_default` text NOT NULL,
  `content_loc1` text,
  `content_loc2` text,
  `content_loc3` text,
  `content_loc4` text,
  `content_loc5` text,
  `content_loc6` text,
  `content_loc7` text,
  `content_loc8` text,
  `sound` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `language` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `emote` smallint(5) unsigned NOT NULL DEFAULT '0',
  `comment` text,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Script Texts';
/*!40101 SET character_set_client = @saved_cs_client */;

LOCK TABLES `script_texts` WRITE;
/*!40000 ALTER TABLE `script_texts` DISABLE KEYS */;
INSERT INTO `script_texts` VALUES (-1000006,'%s becomes enraged!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'EMOTE_BOSS_GENERIC_ENRAGED'),(-1000005,'%s goes into a frenzy!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'EMOTE_BOSS_GENERIC_FRENZY'),(-1000004,'%s goes into a berserker rage!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,2,0,0,'EMOTE_GENERIC_BERSERK'),(-1000003,'%s becomes enraged!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,2,0,0,'EMOTE_GENERIC_ENRAGED'),(-1000002,'%s goes into a frenzy!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,2,0,0,'EMOTE_GENERIC_FRENZY'),(-1000001,'%s goes into a killing frenzy!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,2,0,0,'EMOTE_GENERIC_FRENZY_KILL'),(-1000000,'<ScriptDev2 Text Entry Missing!>',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'DEFAULT_TEXT');
/*!40000 ALTER TABLE `script_texts` ENABLE KEYS */;
UNLOCK TABLES;
DROP TABLE IF EXISTS `script_waypoint`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `script_waypoint` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'creature_template entry',
  `pointid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `location_x` float NOT NULL DEFAULT '0',
  `location_y` float NOT NULL DEFAULT '0',
  `location_z` float NOT NULL DEFAULT '0',
  `waittime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'wait time in milliseconds',
  `point_comment` text,
  PRIMARY KEY (`entry`,`pointid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Script Creature waypoints';
/*!40101 SET character_set_client = @saved_cs_client */;

LOCK TABLES `script_waypoint` WRITE;
/*!40000 ALTER TABLE `script_waypoint` DISABLE KEYS */;
/*!40000 ALTER TABLE `script_waypoint` ENABLE KEYS */;
UNLOCK TABLES;
DROP TABLE IF EXISTS `sd2_db_version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sd2_db_version` (
  `version` varchar(255) NOT NULL DEFAULT '' COMMENT 'Database version string'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

LOCK TABLES `sd2_db_version` WRITE;
/*!40000 ALTER TABLE `sd2_db_version` DISABLE KEYS */;
INSERT INTO `sd2_db_version` VALUES ('ScriptDev2 - Version 0.1.0 (Community Rebuild)');
/*!40000 ALTER TABLE `sd2_db_version` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

