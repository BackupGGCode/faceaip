-- MySQL dump 10.11
--
-- Host: localhost    Database: tshome
-- ------------------------------------------------------
-- Server version	5.0.95

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

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `id` int(11) NOT NULL,
  `username` varchar(32) default NULL,
  `pwd` varchar(32) default NULL,
  `homeid` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES (1,'1234567890','123456789012',1),(2,'123456789','123456789012',200),(3,'100','123456789012',100),(15,'200','123456789012',1),(5,'sunzq','123456',100),(6,'renjy','123456',100),(7,'pan','pan',1),(8,'sunzhiqiang','123456',200),(9,'shihb','123456',200),(10,'songsp','123456',200),(11,'123','123456789012',100),(12,'1234','123456789012',100),(101,'song','myhome',1),(13,'3000','300',1),(16,'12345','123456789012',1),(17,'310','310',10),(29,'311','311',11),(18,'309','309',9),(19,'308','308',8),(20,'307','307',7),(21,'306','306',6),(22,'305','305',5),(23,'304','304',4),(24,'303','303',3),(25,'mahoo','mahoo',1),(26,'mayj','mayj',2),(0,'123456789012','123456789012',2),(30,'1234567890123456789012345678901','1234567890123456789012345678901',1),(28,'sun','myhome',1),(31,'1234567890123456','1234567890123456',1),(27,'300','300',502),(32,'12345678901234567890123456789012','12345678901234567890123456789012',1),(33,'tshome','myhome',502),(34,'tslite','myhome',502),(35,'mylite','myhome',501),(36,'myhome','myhome',501),(37,'renjy','renjy',20),(38,'renjyl','renjyl',20);
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-06-07 10:59:54
