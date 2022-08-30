-- MySQL dump 10.13  Distrib 5.5.62, for Win64 (AMD64)
--
-- ------------------------------------------------------
-- Server version	5.5.5-10.5.15-MariaDB-0+deb11u1

--
-- Table structure for table `SERVER`
--

DROP TABLE IF EXISTS `SERVER`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SERVER` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT current_timestamp(),
  `nom` varchar(100) DEFAULT NULL,
  `os` varchar(255) DEFAULT NULL,
  `machine` varchar(255) DEFAULT NULL,
  `datesys` datetime DEFAULT NULL,
  `ip` varchar(255) DEFAULT NULL,
  `dns` varchar(255) DEFAULT NULL,
  `mac` varchar(255) DEFAULT NULL,
  `info` varchar(2500) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `SERVERTEST`
--

DROP TABLE IF EXISTS `SERVERTEST`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SERVERTEST` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT current_timestamp(),
  `nom` varchar(100) DEFAULT NULL,
  `os` varchar(255) DEFAULT NULL,
  `machine` varchar(255) DEFAULT NULL,
  `datesys` datetime DEFAULT NULL,
  `ip` varchar(255) DEFAULT NULL,
  `dns` varchar(255) DEFAULT NULL,
  `mac` varchar(255) DEFAULT NULL,
  `info` varchar(2500) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
