/*M!999999\- enable the sandbox mode */ 
-- MariaDB dump 10.19-12.2.2-MariaDB, for Win64 (AMD64)
--
-- Host: localhost    Database: proyectofn
-- ------------------------------------------------------
-- Server version	12.2.2-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*M!100616 SET @OLD_NOTE_VERBOSITY=@@NOTE_VERBOSITY, NOTE_VERBOSITY=0 */;

--
-- Table structure for table `categorias`
--

DROP TABLE IF EXISTS `categorias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `categorias` (
  `id_categoria` int(11) NOT NULL AUTO_INCREMENT,
  `nombre_categoria` varchar(50) NOT NULL,
  PRIMARY KEY (`id_categoria`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `categorias`
--

SET @OLD_AUTOCOMMIT=@@AUTOCOMMIT, @@AUTOCOMMIT=0;
LOCK TABLES `categorias` WRITE;
/*!40000 ALTER TABLE `categorias` DISABLE KEYS */;
INSERT INTO `categorias` VALUES
(1,'Carnes y Aves'),
(2,'Pescados y Mariscos'),
(3,'Cereales y Tubérculos'),
(4,'Frutas'),
(5,'Vegetales y Hortalizas'),
(6,'Legumbres (Frijoles)'),
(7,'Lácteos'),
(8,'Huevos'),
(9,'Grasas y Frutos Secos'),
(10,'Azúcares y Postres'),
(11,'Bebidas'),
(12,'Comida Rápida y Antojitos');
/*!40000 ALTER TABLE `categorias` ENABLE KEYS */;
UNLOCK TABLES;
COMMIT;
SET AUTOCOMMIT=@OLD_AUTOCOMMIT;

--
-- Table structure for table `lista_alimentos`
--

DROP TABLE IF EXISTS `lista_alimentos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `lista_alimentos` (
  `id_maestro` int(11) NOT NULL AUTO_INCREMENT,
  `id_categoria` int(11) DEFAULT NULL,
  `id_unidad` int(11) DEFAULT NULL,
  `nombre_alimento` varchar(100) NOT NULL,
  `peso_equivalente_g` decimal(10,2) DEFAULT NULL,
  `calorias` decimal(10,2) DEFAULT NULL,
  `proteinas` decimal(10,2) DEFAULT NULL,
  `carbohidratos` decimal(10,2) DEFAULT NULL,
  `grasas` decimal(10,2) DEFAULT NULL,
  `fibra` decimal(10,2) DEFAULT NULL,
  `azucares` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`id_maestro`),
  KEY `id_categoria` (`id_categoria`),
  KEY `id_unidad` (`id_unidad`),
  CONSTRAINT `1` FOREIGN KEY (`id_categoria`) REFERENCES `categorias` (`id_categoria`),
  CONSTRAINT `2` FOREIGN KEY (`id_unidad`) REFERENCES `unidades_medida` (`id_unidad`)
) ENGINE=InnoDB AUTO_INCREMENT=303 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lista_alimentos`
--

SET @OLD_AUTOCOMMIT=@@AUTOCOMMIT, @@AUTOCOMMIT=0;
LOCK TABLES `lista_alimentos` WRITE;
/*!40000 ALTER TABLE `lista_alimentos` DISABLE KEYS */;
INSERT INTO `lista_alimentos` VALUES
(1,1,1,'Pechuga de Pollo a la plancha',1.00,165.00,31.00,0.00,3.60,0.00,0.00),
(2,1,3,'Muslo de Pollo con piel',150.00,154.60,16.00,0.00,10.00,0.00,0.00),
(3,1,3,'Pierna de Pollo',100.00,191.00,26.00,0.00,9.00,0.00,0.00),
(4,1,3,'Ala de Pollo frita',35.00,290.00,19.00,0.00,19.00,0.00,0.00),
(5,1,1,'Pavo (Pechuga asada)',1.00,135.00,30.00,0.00,1.00,0.00,0.00),
(6,1,1,'Pato (Carne asada)',1.00,337.00,19.00,0.00,28.00,0.00,0.00),
(7,1,1,'Pollo desmenuzado',1.00,165.00,31.00,0.00,3.60,0.00,0.00),
(8,1,3,'Chicharrón de Pollo',30.00,300.00,20.00,5.00,22.00,0.00,0.00),
(9,1,1,'Gallina criolla (cocida)',1.00,220.00,25.00,0.00,12.00,0.00,0.00),
(10,1,3,'Nugget de Pollo',20.00,245.00,15.00,15.00,13.00,0.50,0.00),
(11,1,6,'Lomito de Res',28.35,564.37,91.71,0.00,21.16,0.00,0.00),
(12,1,6,'Puyaso (Puyazo)',28.35,881.83,84.66,0.00,59.96,0.00,0.00),
(13,1,6,'Entraña asada',28.35,740.74,88.18,0.00,42.33,0.00,0.00),
(14,1,6,'Rib Eye',28.35,1026.46,84.66,0.00,77.60,0.00,0.00),
(15,1,1,'Carne molida de res (magra)',1.00,170.00,26.00,0.00,7.00,0.00,0.00),
(16,1,1,'Carne molida de res (corriente)',1.00,250.00,22.00,0.00,18.00,0.00,0.00),
(17,1,6,'T-Bone Steak',28.35,881.83,84.66,0.00,59.96,0.00,0.00),
(18,1,1,'Lengua de Res cocida',1.00,224.00,15.00,0.00,16.00,0.00,0.00),
(19,1,1,'Hígado de Res encebollado',1.00,175.00,27.00,4.00,5.00,0.00,0.00),
(20,1,6,'Carne para guisar (Posta)',28.35,634.92,88.18,0.00,28.22,0.00,0.00),
(21,1,1,'Salpicón de Res',1.00,150.00,22.00,2.00,6.00,0.00,0.00),
(22,1,1,'Carpaccio de Res',1.00,120.00,20.00,0.00,4.00,0.00,0.00),
(23,1,6,'Falda de Res (Hilachas)',28.35,740.74,84.66,0.00,42.33,0.00,0.00),
(24,1,1,'Tripa de Res',1.00,95.00,12.00,0.00,5.00,0.00,0.00),
(25,1,6,'Lomo de Cerdo',28.35,511.46,95.24,0.00,12.35,0.00,0.00),
(26,1,6,'Chuleta de Cerdo asada',28.35,846.56,84.66,0.00,52.91,0.00,0.00),
(27,1,6,'Costilla de Cerdo BBQ',28.35,1128.75,70.55,35.27,77.60,0.00,0.00),
(28,1,1,'Chicharrón de cerdo (cuero)',1.00,540.00,60.00,0.00,30.00,0.00,0.00),
(29,1,1,'Carnitas de Cerdo',1.00,260.00,23.00,0.00,18.00,0.00,0.00),
(30,1,3,'Chorizo Colorado',60.00,310.00,18.00,2.00,25.00,0.00,0.00),
(31,1,3,'Longaniza blanca',60.00,280.00,16.00,3.00,22.00,0.00,0.00),
(32,1,3,'Salchicha de Frankfurt',50.00,270.00,12.00,2.00,24.00,0.00,0.00),
(33,1,3,'Tocino (Bacon) frito',15.00,541.00,37.00,1.40,42.00,0.00,0.00),
(34,1,1,'Jamón Cocido',1.00,105.00,18.00,1.50,3.00,0.00,0.00),
(35,1,1,'Jamón Serrano',1.00,240.00,30.00,0.00,13.00,0.00,0.00),
(36,1,3,'Salami',10.00,330.00,22.00,1.00,26.00,0.00,0.00),
(37,1,3,'Butifarra',50.00,300.00,14.00,2.00,26.00,0.00,0.00),
(38,1,1,'Adobado de Cerdo',1.00,210.00,22.00,3.00,12.00,0.00,0.00),
(39,1,6,'Carne de Cordero (Pierna)',28.35,705.47,88.18,0.00,35.27,0.00,0.00),
(40,1,6,'Cordero (Chuletas)',28.35,917.11,81.13,0.00,63.49,0.00,0.00),
(41,1,1,'Conejo (Carne cocida)',1.00,175.00,28.00,0.00,6.00,0.00,0.00),
(42,1,1,'Codorniz asada',1.00,230.00,25.00,0.00,14.00,0.00,0.00),
(43,1,1,'Carne de Venado',1.00,158.00,30.00,0.00,3.20,0.00,0.00),
(44,1,1,'Pastrami',1.00,147.00,22.00,1.00,6.00,0.00,0.00),
(45,1,3,'Mortadela',20.00,310.00,12.00,3.00,28.00,0.00,0.00),
(46,1,1,'Carne de Cabrito',1.00,143.00,27.00,0.00,3.00,0.00,0.00),
(47,1,6,'Bistec de Res (Suave)',28.35,634.92,91.71,0.00,28.22,0.00,0.00),
(48,1,1,'Pechuga de Pollo Empanizada',1.00,260.00,22.00,14.00,12.00,1.00,0.00),
(49,1,1,'Tasajo de Res',1.00,250.00,45.00,0.00,7.00,0.00,0.00),
(50,1,3,'Patas de Pollo cocidas',30.00,215.00,19.00,0.20,15.00,0.00,0.00),
(51,2,6,'Filete de Tilapia a la plancha',28.35,451.50,91.71,0.00,9.52,0.00,0.00),
(52,2,6,'Filete de Robalo',28.35,320.99,65.26,0.00,4.23,0.00,0.00),
(53,2,6,'Filete de Bacalao',28.35,289.24,63.49,0.00,2.47,0.00,0.00),
(54,2,6,'Filete de Mero',28.35,324.51,67.02,0.00,3.53,0.00,0.00),
(55,2,6,'Filete de Lenguado',28.35,320.99,66.31,0.00,4.23,0.00,0.00),
(56,2,6,'Corvina asada',28.35,366.84,70.55,0.00,8.82,0.00,0.00),
(57,2,3,'Pescado Blanco entero (mediano)',250.00,110.00,20.00,0.00,2.00,0.00,0.00),
(58,2,1,'Pescado Seco (Bacalao salado)',1.00,290.00,62.00,0.00,4.00,0.00,0.00),
(59,2,6,'Filete de Salmón Atlántico',28.35,733.69,70.55,0.00,45.86,0.00,0.00),
(60,2,6,'Atún fresco (Bistec)',28.35,507.94,81.13,0.00,17.28,0.00,0.00),
(61,2,3,'Atún en lata (en agua)',140.00,116.00,26.00,0.00,0.80,0.00,0.00),
(62,2,3,'Atún en lata (en aceite)',140.00,198.00,24.00,0.00,11.00,0.00,0.00),
(63,2,3,'Sardinas en tomate (lata)',120.00,185.00,21.00,1.00,10.00,0.00,0.00),
(64,2,6,'Trucha de río',28.35,522.05,74.07,0.00,21.16,0.00,0.00),
(65,2,6,'Caballa (Mackerel)',28.35,723.10,67.02,0.00,49.38,0.00,0.00),
(66,2,3,'Camarón Gigante (Jumbo)',30.00,99.00,21.00,0.00,1.00,0.00,0.00),
(67,2,3,'Camarón mediano (cocido)',10.00,99.00,21.00,0.00,1.00,0.00,0.00),
(68,2,1,'Camarón seco',1.00,250.00,50.00,0.00,3.00,0.00,0.00),
(69,2,3,'Langosta (Cola entera)',150.00,90.00,19.00,0.00,1.00,0.00,0.00),
(70,2,3,'Cangrejo (Muela/Pata)',50.00,84.00,18.00,0.00,1.00,0.00,0.00),
(71,2,1,'Surimi (Palitos de cangrejo)',1.00,99.00,15.00,7.00,1.00,0.00,0.00),
(72,2,3,'Langostino',40.00,105.00,20.00,0.50,1.50,0.00,0.00),
(73,2,3,'Ostra fresca',20.00,68.00,7.00,4.00,2.00,0.00,0.00),
(74,2,3,'Almeja blanca',15.00,74.00,12.00,2.50,1.00,0.00,0.00),
(75,2,1,'Pulpo cocido',1.00,164.00,30.00,4.00,2.00,0.00,0.00),
(76,2,1,'Calamar (Anillos)',1.00,175.00,18.00,3.00,7.00,0.00,0.00),
(77,2,1,'Calamar frito (Rebozado)',1.00,250.00,15.00,15.00,13.00,0.50,0.00),
(78,2,3,'Mejillón en su concha',25.00,86.00,12.00,3.50,2.00,0.00,0.00),
(79,2,3,'Vieira (Scallop)',25.00,111.00,20.00,5.00,0.80,0.00,0.00),
(80,2,1,'Caracol de mar cocido',1.00,130.00,25.00,2.00,1.00,0.00,0.00),
(81,2,1,'Ceviche de Pescado',1.00,95.00,15.00,5.00,1.50,0.50,0.00),
(82,2,1,'Ceviche de Camarón',1.00,110.00,14.00,8.00,2.00,0.50,0.00),
(83,2,3,'Mojarra frita entera',350.00,250.00,22.00,2.00,15.00,0.00,0.00),
(84,2,3,'Pescado empanizado (Filete)',120.00,230.00,16.00,18.00,12.00,1.00,0.00),
(85,2,1,'Sopa de Mariscos (Sólidos)',1.00,120.00,18.00,5.00,4.00,0.50,0.00),
(86,2,3,'Medallón de Tiburón (Cazón)',150.00,130.00,21.00,0.00,4.50,0.00,0.00),
(87,2,6,'Pez Espada',28.35,606.70,70.55,0.00,35.27,0.00,0.00),
(88,2,1,'Anchoas en aceite',1.00,210.00,29.00,0.00,10.00,0.00,0.00),
(89,2,6,'Filete de Huachinango (Pargo)',28.35,451.50,91.71,0.00,6.00,0.00,0.00),
(90,2,1,'Pescado al vapor con jengibre',1.00,100.00,20.00,1.00,2.00,0.00,0.00),
(91,2,3,'Tortitas de camarón seco',40.00,280.00,18.00,12.00,20.00,0.50,0.00),
(92,2,1,'Salmón ahumado',1.00,117.00,18.00,0.00,4.00,0.00,0.00),
(93,2,3,'Camarones al ajillo',100.00,150.00,18.00,2.00,8.00,0.00,0.00),
(94,2,1,'Carpaccio de Salmón',1.00,180.00,20.00,0.00,11.00,0.00,0.00),
(95,2,3,'Sushi (Nigiri de pescado)',30.00,150.00,6.00,28.00,0.50,0.20,0.00),
(96,2,1,'Mousse de salmón',1.00,210.00,12.00,4.00,16.00,0.00,0.00),
(97,2,3,'Bolas de pescado (Fish balls)',20.00,100.00,12.00,8.00,2.00,0.00,0.00),
(98,2,6,'Anguila cocida',28.35,832.45,81.13,0.00,52.91,0.00,0.00),
(99,2,1,'Erizos de mar',1.00,120.00,13.00,5.00,5.00,0.00,0.00),
(100,2,3,'Tenazas de cangrejo moro',80.00,95.00,20.00,0.00,1.50,0.00,0.00),
(101,3,4,'Arroz Blanco cocido',200.00,130.00,2.70,28.00,0.30,0.40,0.10),
(102,3,4,'Arroz Integral cocido',195.00,112.00,2.60,23.50,0.90,1.80,0.10),
(103,3,4,'Avena en hojuelas (cocida con agua)',240.00,71.00,2.50,12.00,1.40,1.70,0.50),
(104,3,4,'Quinoa cocida',185.00,120.00,4.40,21.30,1.90,2.80,0.00),
(105,3,4,'Cebada cocida',157.00,123.00,2.30,28.00,0.40,3.80,0.00),
(106,3,4,'Cuscús cocido',157.00,112.00,3.80,23.00,0.20,1.40,0.00),
(107,3,4,'Maíz dulce desgranado',160.00,86.00,3.20,19.00,1.20,2.70,0.00),
(108,3,3,'Tortilla de Maíz blanca',30.00,218.00,5.00,45.00,2.50,6.00,0.90),
(109,3,3,'Tortilla de Maíz negra',35.00,220.00,6.00,44.00,3.00,7.00,0.90),
(110,3,3,'Tamal de elote',150.00,150.00,3.00,28.00,4.00,2.00,0.00),
(111,3,3,'Chuchito (solo masa)',100.00,200.00,4.00,30.00,8.00,2.50,0.00),
(112,3,3,'Tamal colorado (solo masa)',200.00,180.00,3.50,25.00,10.00,2.00,0.00),
(113,3,1,'Masa de maíz nixtamalizado',1.00,199.00,4.00,40.00,2.50,5.00,0.00),
(114,3,3,'Elote asado (unidad)',180.00,90.00,3.00,19.00,1.00,2.00,0.00),
(115,3,3,'Papa cocida (mediana)',150.00,77.00,2.00,17.50,0.10,2.20,0.00),
(116,3,4,'Puré de Papa (con leche)',210.00,88.00,2.00,15.00,2.50,1.50,0.00),
(117,3,1,'Papas fritas (french fries)',1.00,312.00,3.40,41.00,15.00,3.80,0.00),
(118,3,3,'Camote asado',130.00,86.00,1.60,20.00,0.10,3.00,0.00),
(119,3,1,'Yuca cocida',1.00,160.00,1.40,38.00,0.30,1.80,0.00),
(120,3,1,'Malanga cocida',1.00,142.00,1.50,34.00,0.10,5.10,0.00),
(121,3,1,'Güisquil cocido (porción)',1.00,19.00,0.80,4.50,0.10,1.70,0.00),
(122,3,1,'Ñame cocido',1.00,118.00,1.50,27.00,0.20,4.10,0.00),
(123,3,4,'Pasta Espagueti cocido',140.00,158.00,5.80,31.00,0.90,1.80,0.00),
(124,3,4,'Pasta Integral cocida',140.00,124.00,5.30,26.50,0.50,3.90,0.00),
(125,3,4,'Macarrones con queso',200.00,164.00,7.00,20.00,6.00,0.50,0.00),
(126,3,4,'Lasaña de carne (porción)',250.00,135.00,8.00,14.00,5.00,1.00,0.00),
(127,3,1,'Harina de Trigo todo uso',1.00,364.00,10.00,76.00,1.00,2.70,0.00),
(128,3,1,'Pan de manteca (Guate)',1.00,290.00,8.00,55.00,5.00,2.00,0.00),
(129,3,3,'Pan Frances (unidad)',40.00,290.00,9.00,56.00,3.00,2.40,0.00),
(130,3,3,'Pan Integral (rodaja)',28.00,247.00,13.00,41.00,3.40,7.00,0.00),
(131,3,3,'Pan para Hamburguesa',50.00,279.00,9.00,49.00,4.50,2.00,0.00),
(132,3,3,'Champurrada (unidad)',30.00,450.00,6.00,65.00,18.00,1.50,0.00),
(133,3,4,'Cereal de hojuelas de maíz (Cornflakes)',30.00,357.00,7.00,83.00,0.40,3.30,0.00),
(134,3,4,'Cereal inflado con azúcar',30.00,390.00,5.00,90.00,1.00,1.00,0.00),
(135,3,1,'Galletas de soda',1.00,421.00,9.00,71.00,11.00,2.30,0.00),
(136,3,1,'Galletas integrales',1.00,440.00,8.00,66.00,16.00,6.00,0.00),
(137,3,4,'Poporopos (Palomitas naturales)',8.00,387.00,13.00,78.00,4.50,14.50,0.00),
(138,3,1,'Mosh (Avena con leche)',1.00,85.00,3.50,12.00,2.50,1.00,0.00),
(139,3,3,'Panqueque (unidad)',40.00,227.00,6.00,28.00,10.00,0.00,0.00),
(140,3,3,'Waffle (unidad)',75.00,291.00,8.00,33.00,14.00,1.00,0.00),
(141,3,1,'Salvado de trigo',1.00,216.00,15.60,64.50,4.30,42.80,0.00),
(142,3,1,'Germen de trigo',1.00,360.00,23.00,51.00,10.00,13.00,0.00),
(143,3,4,'Arroz con leche',200.00,111.00,3.30,22.00,0.90,0.00,0.10),
(144,3,1,'Tapioca cocida',1.00,159.00,0.20,39.00,0.00,0.00,0.00),
(145,3,1,'Granola con miel',1.00,471.00,10.00,64.00,20.00,5.00,0.00),
(146,3,3,'Muffin de avena',60.00,285.00,6.00,45.00,10.00,3.00,0.00),
(147,3,3,'Tortilla de harina (grande)',45.00,310.00,8.00,50.00,8.00,2.00,0.00),
(148,3,4,'Polenta cocida',230.00,64.00,1.50,13.50,0.20,1.00,0.00),
(149,3,3,'Bagel natural',85.00,250.00,10.00,48.00,1.50,2.20,0.00),
(150,3,1,'Grisines (Palitroques)',1.00,408.00,12.00,72.00,8.00,3.00,0.00),
(151,4,3,'Banano maduro (mediano)',118.00,89.00,1.10,22.80,0.30,2.60,12.20),
(152,4,3,'Manzana Roja (con cáscara)',182.00,52.00,0.30,13.80,0.20,2.40,10.40),
(153,4,3,'Naranja (mediana)',131.00,47.00,0.90,11.80,0.10,2.40,9.35),
(154,4,3,'Pera (mediana)',178.00,57.00,0.40,15.20,0.10,3.10,0.00),
(155,4,3,'Durazno (mediano)',150.00,39.00,0.90,9.50,0.30,1.50,0.00),
(156,4,3,'Ciruela Roja',66.00,46.00,0.70,11.40,0.30,1.40,0.00),
(157,4,3,'Mandarina (mediana)',88.00,53.00,0.80,13.30,0.30,1.80,0.00),
(158,4,3,'Kiwi (sin cáscara)',69.00,61.00,1.10,14.70,0.50,3.00,0.00),
(159,4,4,'Piña picada',165.00,50.00,0.50,13.10,0.10,1.40,9.85),
(160,4,4,'Sandía picada',152.00,30.00,0.60,7.60,0.20,0.40,0.00),
(161,4,4,'Papaya picada',145.00,43.00,0.50,10.80,0.30,1.70,0.00),
(162,4,4,'Melón picado',160.00,34.00,0.80,8.20,0.20,0.90,0.00),
(163,4,4,'Fresas enteras',144.00,32.00,0.70,7.70,0.30,2.00,0.00),
(164,4,4,'Uvas Rojas/Verdes',151.00,69.00,0.70,18.10,0.20,0.90,15.50),
(165,4,4,'Mango picado',165.00,60.00,0.80,15.00,0.40,1.60,13.70),
(166,4,4,'Arándanos (Blueberries)',148.00,57.00,0.70,14.50,0.30,2.40,0.00),
(167,4,3,'Aguacate (mediano)',200.00,160.00,2.00,8.50,14.70,6.70,0.00),
(168,4,3,'Limón (unidad)',58.00,29.00,1.10,9.30,0.30,2.80,0.00),
(169,4,4,'Coco rallado (sin azúcar)',80.00,354.00,3.30,15.20,33.50,9.00,0.00),
(170,4,1,'Higo seco',1.00,249.00,3.30,63.90,0.90,9.80,0.00),
(171,5,3,'Tomate de cocina (mediano)',123.00,18.00,0.90,3.90,0.20,1.20,0.00),
(172,5,3,'Zanahoria (mediana)',61.00,41.00,0.90,9.60,0.20,2.80,0.00),
(173,5,3,'Pepino con cáscara (mediano)',200.00,15.00,0.70,3.60,0.10,0.50,0.00),
(174,5,3,'Cebolla blanca (mediana)',110.00,40.00,1.10,9.30,0.10,1.70,0.00),
(175,5,3,'Diente de Ajo',3.00,149.00,6.40,33.10,0.50,2.10,0.00),
(176,5,3,'Chile Pimiento Verde (mediano)',119.00,20.00,0.90,4.60,0.20,1.70,0.00),
(177,5,3,'Rábano (unidad)',4.50,16.00,0.70,3.40,0.10,1.60,0.00),
(178,5,4,'Brócoli picado (cocido)',156.00,35.00,2.40,7.20,0.40,3.30,0.00),
(179,5,4,'Espinaca cruda (hojas)',30.00,23.00,2.90,3.60,0.40,2.20,0.00),
(180,5,4,'Lechuga Iceberg (picada)',72.00,14.00,0.90,3.00,0.10,1.20,0.00),
(181,5,4,'Repollo blanco (picado)',70.00,25.00,1.30,5.80,0.10,2.50,0.00),
(182,5,4,'Coliflor (cocida)',124.00,23.00,1.80,4.10,0.50,2.30,0.00),
(183,5,4,'Ejotes (cocidos)',125.00,31.00,1.80,7.00,0.10,3.40,0.00),
(184,5,4,'Calabacín o Zucchini (picado)',180.00,17.00,1.20,3.10,0.30,1.00,0.00),
(185,5,4,'Hongos o Champiñones',70.00,22.00,3.10,3.30,0.30,1.00,0.00),
(186,5,3,'Güisquil cocido (mitad)',150.00,19.00,0.80,4.50,0.10,1.70,0.00),
(187,5,4,'Picado de Rábano (preparado)',100.00,20.00,0.80,4.00,0.10,1.80,0.00),
(188,5,1,'Cilantro o Perejil',1.00,23.00,2.10,3.70,0.50,2.80,0.00),
(189,5,3,'Berenjena asada (rodaja)',20.00,25.00,1.00,6.00,0.20,3.00,0.00),
(190,5,3,'Apio (tallo mediano)',40.00,16.00,0.70,3.00,0.20,1.60,0.00),
(191,6,4,'Frijoles Negros cocidos (enteros)',172.00,132.00,8.90,23.70,0.50,8.70,0.00),
(192,6,4,'Frijoles Negros volteos (refritos)',200.00,180.00,7.50,20.00,8.00,7.00,0.00),
(193,6,4,'Frijoles Colorados cocidos',177.00,127.00,8.70,22.80,0.50,7.40,0.00),
(194,6,4,'Frijoles Blancos cocidos',180.00,139.00,9.70,25.00,0.40,6.30,0.00),
(195,6,4,'Lentejas cocidas',198.00,116.00,9.00,20.10,0.40,7.90,0.00),
(196,6,4,'Garbanzos cocidos',164.00,164.00,8.90,27.40,2.60,7.60,0.00),
(197,6,4,'Habas cocidas',170.00,110.00,7.60,19.70,0.40,5.40,0.00),
(198,6,4,'Arvejas o Guisantes (cocidos)',160.00,81.00,5.40,14.50,0.40,5.70,0.00),
(199,6,1,'Soya (grano cocido)',1.00,173.00,16.60,9.90,9.00,6.00,0.00),
(200,6,1,'Hummus (puré de garbanzo)',1.00,166.00,7.90,14.30,9.60,6.00,0.00),
(201,7,2,'Leche Entera',1.00,61.00,3.20,4.80,3.30,0.00,4.80),
(202,7,2,'Leche Descremada (0%)',1.00,34.00,3.40,5.00,0.10,0.00,5.00),
(203,7,2,'Leche Semidescremada (2%)',1.00,50.00,3.30,4.80,2.00,0.00,0.00),
(204,7,2,'Leche sin Lactosa',1.00,52.00,3.20,5.00,2.50,0.00,0.00),
(205,7,1,'Queso Fresco (Tipo panela)',1.00,299.00,18.00,3.00,24.00,0.00,0.00),
(206,7,1,'Queso Crema',1.00,342.00,6.00,4.00,34.00,0.00,0.00),
(207,7,1,'Queso Seco (Zacapa)',1.00,380.00,25.00,2.00,30.00,0.00,0.00),
(208,7,1,'Queso Mozzarella',1.00,280.00,22.00,2.20,20.00,0.00,0.00),
(209,7,1,'Queso Cheddar',1.00,403.00,25.00,1.30,33.00,0.00,0.00),
(210,7,1,'Quesillo (para pupusas/tortillas)',1.00,285.00,20.00,3.00,21.00,0.00,0.00),
(211,7,3,'Queso de Capa (rodaja)',30.00,250.00,18.00,2.00,19.00,0.00,0.00),
(212,7,4,'Yogur Natural (sin azúcar)',245.00,61.00,3.50,4.70,3.30,0.00,4.70),
(213,7,4,'Yogur Griego natural',245.00,59.00,10.00,3.60,0.40,0.00,0.00),
(214,7,4,'Yogur con Fruta (con azúcar)',245.00,105.00,3.00,18.00,2.50,0.20,15.00),
(215,7,1,'Crema fresca (Guate)',1.00,195.00,2.50,3.50,20.00,0.00,0.00),
(216,7,1,'Mantequilla con sal',1.00,717.00,0.90,0.10,81.00,0.00,0.00),
(217,7,3,'Margarina (cucharada)',14.00,713.00,0.00,0.00,81.00,0.00,0.00),
(218,7,2,'Leche Condensada',1.00,321.00,7.90,54.00,8.70,0.00,0.00),
(219,7,2,'Leche Evaporada',1.00,131.00,6.80,10.00,7.60,0.00,0.00),
(220,8,3,'Huevo de Gallina (Entero crudo)',50.00,155.00,13.00,1.10,11.00,0.00,0.00),
(221,8,3,'Huevo Cocido (Duro)',50.00,155.00,13.00,1.10,11.00,0.00,0.00),
(222,8,3,'Huevo Frito (con aceite)',50.00,196.00,13.00,1.20,15.00,0.00,0.00),
(223,8,3,'Huevo Revuelto (natural)',50.00,170.00,12.00,1.50,12.00,0.00,0.00),
(224,8,3,'Huevo Poché (Escalfado)',50.00,143.00,12.50,0.70,9.50,0.00,0.00),
(225,8,3,'Clara de Huevo (unidad)',33.00,52.00,11.00,0.70,0.20,0.00,0.00),
(226,8,3,'Yema de Huevo (unidad)',17.00,322.00,16.00,3.60,26.50,0.00,0.00),
(227,8,3,'Huevo con Tomate y Cebolla (Guate)',60.00,160.00,11.00,3.00,11.50,0.50,0.00),
(228,8,3,'Huevo con Chorizo (por unidad de huevo)',70.00,250.00,14.00,2.50,20.00,0.10,0.00),
(229,8,3,'Omelette de Queso (2 huevos)',120.00,210.00,15.00,2.00,16.00,0.00,0.00),
(230,8,1,'Huevo en Polvo (deshidratado)',1.00,492.00,48.00,4.00,31.00,0.00,0.00),
(231,8,3,'Huevo de Codorniz (unidad)',9.00,158.00,13.00,0.40,11.00,0.00,0.00),
(232,9,5,'Aceite de Oliva',14.00,884.00,0.00,0.00,100.00,0.00,0.00),
(233,9,5,'Aceite Vegetal (Girasol/Maíz)',14.00,884.00,0.00,0.00,100.00,0.00,0.00),
(234,9,5,'Aceite de Coco',14.00,862.00,0.00,0.00,100.00,0.00,0.00),
(235,9,5,'Manteca de Cerdo',13.00,902.00,0.00,0.00,100.00,0.00,0.00),
(236,9,5,'Mayonesa comercial',15.00,680.00,1.00,0.60,75.00,0.00,0.00),
(237,9,1,'Maní tostado (sin sal)',1.00,567.00,25.80,16.10,49.20,8.50,0.00),
(238,9,3,'Almendra (unidad)',1.20,579.00,21.20,21.70,49.90,12.50,0.00),
(239,9,3,'Nuez de nogal (unidad/mitad)',2.00,654.00,15.20,13.70,65.20,6.70,0.00),
(240,9,1,'Pistachos (sin cáscara)',1.00,562.00,20.20,27.50,45.30,10.60,0.00),
(241,9,1,'Cashews (Anacardos/Marañón)',1.00,553.00,18.20,30.20,43.80,3.30,0.00),
(242,9,3,'Macadamia (unidad)',3.00,718.00,7.90,13.80,75.80,8.60,0.00),
(243,9,1,'Avellanas',1.00,628.00,15.00,16.70,60.80,9.70,0.00),
(244,9,5,'Semillas de Girasol (Pepitas)',10.00,584.00,20.80,20.00,51.50,8.60,0.00),
(245,9,5,'Semillas de Calabaza (Pepitoria)',10.00,559.00,30.20,10.70,49.00,6.00,0.00),
(246,9,5,'Semillas de Chía',10.00,486.00,16.50,42.10,30.70,34.40,0.00),
(247,9,5,'Semillas de Sésamo (Ajonjolí)',10.00,573.00,17.70,23.40,49.70,11.80,0.00),
(248,9,5,'Mantequilla de Maní (Peanut Butter)',16.00,588.00,25.00,20.00,50.00,6.00,0.00),
(249,9,1,'Pate de hígado',1.00,327.00,14.00,1.50,29.00,0.00,0.00),
(250,9,1,'Tahini (Pasta de sésamo)',1.00,595.00,17.00,21.00,53.00,9.00,0.00),
(251,9,3,'Aceituna Verde (unidad)',4.00,115.00,0.80,6.30,10.70,3.20,0.00),
(252,10,5,'Azúcar Blanca',15.00,387.00,0.00,100.00,0.00,0.00,99.80),
(253,10,5,'Azúcar Morena',15.00,380.00,0.00,98.00,0.00,0.00,97.00),
(254,10,5,'Miel de Abeja',21.00,304.00,0.30,82.00,0.00,0.00,82.00),
(255,10,5,'Jarabe de Membrillo / Maple',20.00,260.00,0.00,67.00,0.10,0.00,0.00),
(256,10,3,'Panela / Rapadura (trozo)',20.00,312.00,0.00,81.00,0.00,0.00,0.00),
(257,10,3,'Rellenito de Plátano (unidad)',100.00,220.00,3.50,45.00,4.00,3.00,0.00),
(258,10,3,'Mole de Plátano (porción)',150.00,280.00,4.00,55.00,8.00,3.50,0.00),
(259,10,3,'Arroz con Leche (taza)',200.00,111.00,3.30,22.00,0.90,0.00,0.10),
(260,10,3,'Torreja / Mollete (unidad)',120.00,350.00,6.00,60.00,12.00,1.00,0.00),
(261,10,3,'Barquillo (unidad)',10.00,400.00,5.00,75.00,10.00,0.50,0.00),
(262,10,3,'Chocolate de Leche (barra)',40.00,535.00,7.60,59.00,30.00,3.40,51.00),
(263,10,1,'Chocolate Amargo (70% cacao)',1.00,598.00,7.80,45.00,42.00,11.00,0.00),
(264,10,1,'Nutella / Crema de avellana',1.00,539.00,6.30,57.00,31.00,3.50,0.00),
(265,10,3,'Dona Glaseada',60.00,452.00,4.90,51.00,25.00,1.00,0.00),
(266,10,3,'Galleta Oreo (unidad)',11.00,480.00,4.00,70.00,20.00,2.50,0.00),
(267,10,3,'Malvavisco / Angelito (unidad)',7.00,318.00,1.80,81.00,0.20,0.00,0.00),
(268,10,1,'Gomitas de dulce',1.00,316.00,6.00,77.00,0.00,0.00,0.00),
(269,10,4,'Helado de Vainilla (taza)',150.00,201.00,3.50,24.00,11.00,0.00,21.00),
(270,10,4,'Sorbete de Frutas (taza)',150.00,144.00,0.00,32.00,0.00,1.50,0.00),
(271,10,3,'Pie de Queso (rebanada)',120.00,321.00,5.50,25.00,22.00,0.50,0.00),
(272,11,2,'Agua Pura',1.00,0.00,0.00,0.00,0.00,0.00,0.00),
(273,11,2,'Café Negro (sin azúcar)',1.00,1.00,0.10,0.00,0.00,0.00,0.00),
(274,11,2,'Té natural (sin azúcar)',1.00,1.00,0.00,0.20,0.00,0.00,0.00),
(275,11,3,'Gaseosa / Soda (lata)',355.00,42.00,0.00,10.60,0.00,0.00,10.60),
(276,11,3,'Gaseosa Dieta / Zero (lata)',355.00,0.30,0.00,0.10,0.00,0.00,0.00),
(277,11,2,'Bebida Energética',1.00,45.00,0.00,11.00,0.00,0.00,0.00),
(278,11,2,'Bebida Deportiva (Isotónica)',1.00,24.00,0.00,6.00,0.00,0.00,0.00),
(279,11,4,'Juego de Naranja natural (taza)',240.00,45.00,0.70,10.40,0.20,0.20,0.00),
(280,11,4,'Limonada con azúcar (taza)',240.00,40.00,0.10,10.00,0.00,0.00,0.00),
(281,11,4,'Licuado de Papaya con agua',240.00,32.00,0.40,8.00,0.20,1.30,0.00),
(282,11,4,'Atol de Elote (taza)',240.00,120.00,2.00,22.00,3.00,1.50,0.00),
(283,11,4,'Incparina preparada (taza)',240.00,110.00,4.00,20.00,1.50,1.00,0.00),
(284,11,2,'Agua de Coco natural',1.00,19.00,0.70,3.70,0.20,1.10,0.00),
(285,11,3,'Cerveza Clara (lata/botella)',355.00,43.00,0.50,3.50,0.00,0.00,0.00),
(286,11,3,'Cerveza Obscura (lata/botella)',355.00,50.00,0.50,4.50,0.00,0.00,0.00),
(287,11,2,'Vino Tinto / Blanco (copa)',150.00,85.00,0.10,2.60,0.00,0.00,0.00),
(288,11,2,'Whisky / Ron / Tequila (shot)',44.00,230.00,0.00,0.10,0.00,0.00,0.00),
(289,11,4,'Chocolate caliente (con leche)',240.00,80.00,3.50,12.00,2.50,0.50,0.00),
(290,11,2,'Leche de Almendras (sin azúcar)',1.00,15.00,0.50,0.30,1.20,0.30,0.00),
(291,11,2,'Leche de Soya',1.00,45.00,3.30,4.00,2.00,0.50,0.00),
(292,12,3,'Pizza de Pepperoni (rebanada)',110.00,298.00,12.00,31.00,14.00,2.00,0.00),
(293,12,3,'Pizza de Queso (rebanada)',100.00,266.00,11.00,30.00,10.00,1.50,0.00),
(294,12,3,'Pizza Suprema (rebanada)',130.00,310.00,13.00,32.00,15.00,3.00,0.00),
(295,12,3,'Hamburguesa con Queso sencilla',150.00,250.00,15.00,30.00,12.00,2.00,0.00),
(296,12,3,'Hamburguesa Doble con Tocino',220.00,320.00,20.00,28.00,22.00,2.00,0.00),
(297,12,3,'Sandwich de Pollo Frito',180.00,280.00,16.00,35.00,14.00,2.50,0.00),
(298,12,3,'Hot Dog con todo (Shuco)',120.00,240.00,10.00,25.00,15.00,1.50,0.00),
(299,12,3,'Papas Fritas (porción mediana)',150.00,312.00,3.40,41.00,15.00,3.80,0.00),
(300,12,3,'Nuggets de Pollo (6 piezas)',100.00,302.00,15.00,16.00,20.00,1.00,0.00),
(301,12,3,'Taco de Carne Asada (unidad)',70.00,210.00,14.00,18.00,9.00,2.00,0.00),
(302,12,3,'Enchilada Guatemalteca',120.00,180.00,8.00,20.00,9.00,3.50,0.00);
/*!40000 ALTER TABLE `lista_alimentos` ENABLE KEYS */;
UNLOCK TABLES;
COMMIT;
SET AUTOCOMMIT=@OLD_AUTOCOMMIT;

--
-- Table structure for table `registros_comida`
--

DROP TABLE IF EXISTS `registros_comida`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `registros_comida` (
  `id_registro` int(11) NOT NULL AUTO_INCREMENT,
  `id_usuario` int(11) DEFAULT NULL,
  `nombre_alimento` varchar(100) NOT NULL,
  `cantidad_proporcional` varchar(50) DEFAULT NULL,
  `tiempo_comida` enum('Desayuno','Almuerzo','Cena','Refaccion') NOT NULL,
  `fecha_ingesta` date DEFAULT curdate(),
  `hora_ingesta` time DEFAULT curtime(),
  `id_unidad` int(11) DEFAULT NULL,
  `nombre_unidad` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id_registro`),
  KEY `id_usuario` (`id_usuario`),
  KEY `fk_unidad_registro` (`id_unidad`),
  CONSTRAINT `1` FOREIGN KEY (`id_usuario`) REFERENCES `usuarios` (`id`) ON DELETE CASCADE,
  CONSTRAINT `fk_unidad_registro` FOREIGN KEY (`id_unidad`) REFERENCES `unidades_medida` (`id_unidad`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=108 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `registros_comida`
--

SET @OLD_AUTOCOMMIT=@@AUTOCOMMIT, @@AUTOCOMMIT=0;
LOCK TABLES `registros_comida` WRITE;
/*!40000 ALTER TABLE `registros_comida` DISABLE KEYS */;
INSERT INTO `registros_comida` VALUES
(4,1,'Huevo de Gallina (Entero crudo)','2','Desayuno','2026-04-20','08:00:00',3,'Unidad'),
(5,1,'Pan Frances (unidad)','2','Desayuno','2026-04-20','08:05:00',3,'Unidad'),
(6,1,'Frijoles Negros volteos (refritos)','1','Desayuno','2026-04-20','08:10:00',4,'Taza'),
(7,1,'Cafe Negro (sin azucar)','1','Desayuno','2026-04-20','08:15:00',4,'Taza'),
(8,1,'Pechuga de Pollo a la plancha','150','Almuerzo','2026-04-20','13:00:00',1,'Gramos'),
(9,1,'Arroz Blanco cocido','1','Almuerzo','2026-04-20','13:10:00',4,'Taza'),
(10,1,'Ensalada de Lechuga','1','Almuerzo','2026-04-20','13:15:00',4,'Taza'),
(11,1,'Manzana Roja (con cáscara)','1','Refaccion','2026-04-20','16:00:00',3,'Unidad'),
(12,1,'Avena en hojuelas (cocida con agua)','1.5','Desayuno','2026-04-21','07:30:00',4,'Taza'),
(13,1,'Leche Descremada (0%)','250','Desayuno','2026-04-21','07:35:00',1,'Gramos'),
(14,1,'Lomito de Res','8','Almuerzo','2026-04-21','13:30:00',6,'Onzas'),
(15,1,'Papa cocida (mediana)','2','Almuerzo','2026-04-21','13:40:00',3,'Unidad'),
(16,1,'Gaseosa / Soda (lata)','1','Almuerzo','2026-04-21','13:45:00',3,'Unidad'),
(17,1,'Yogur Griego natural','1','Cena','2026-04-21','19:00:00',4,'Taza'),
(18,1,'Omelette de Queso (2 huevos)','1','Desayuno','2026-04-22','08:00:00',3,'Unidad'),
(19,1,'Tortilla de Maíz blanca','3','Desayuno','2026-04-22','08:10:00',3,'Unidad'),
(20,1,'Nugget de Pollo','6','Almuerzo','2026-04-22','12:30:00',3,'Unidad'),
(21,1,'Papas fritas (french fries)','150','Almuerzo','2026-04-22','12:40:00',1,'Gramos'),
(22,1,'Cerveza Clara (lata/botella)','2','Cena','2026-04-22','20:00:00',3,'Unidad'),
(23,1,'Pizza de Pepperoni (rebanada)','3','Cena','2026-04-22','20:15:00',3,'Unidad'),
(24,1,'Mosh (Avena con leche)','1','Desayuno','2026-04-23','07:00:00',4,'Taza'),
(25,1,'Banano maduro (mediano)','1','Desayuno','2026-04-23','07:15:00',3,'Unidad'),
(26,1,'Pollo desmenuzado','200','Almuerzo','2026-04-23','13:00:00',1,'Gramos'),
(27,1,'Aguacate (mediano)','0.5','Almuerzo','2026-04-23','13:10:00',3,'Unidad'),
(28,1,'Jugo de Naranja natural (taza)','1','Almuerzo','2026-04-23','13:15:00',4,'Taza'),
(29,1,'Almendra (unidad)','15','Refaccion','2026-04-23','17:00:00',3,'Unidad'),
(30,1,'Atún en agua (lata)','1','Cena','2026-04-23','19:30:00',3,'Unidad'),
(31,1,'Pan Integral (rodaja)','2','Desayuno','2026-04-24','08:00:00',3,'Unidad'),
(32,1,'Queso Fresco (Tipo panela)','2','Desayuno','2026-04-24','08:05:00',6,'Onzas'),
(33,1,'Pescado al vapor con jengibre','250','Almuerzo','2026-04-24','13:30:00',1,'Gramos'),
(34,1,'Brócoli picado (cocido)','1','Almuerzo','2026-04-24','13:40:00',4,'Taza'),
(35,1,'Te natural (sin azúcar)','1','Almuerzo','2026-04-24','13:45:00',4,'Taza'),
(36,1,'Mango picado','1','Refaccion','2026-04-24','16:00:00',4,'Taza'),
(37,1,'Sopa de Mariscos (Sólidos)','1.5','Cena','2026-04-24','19:00:00',4,'Taza'),
(38,1,'Tamal de elote','1','Desayuno','2026-04-25','09:00:00',3,'Unidad'),
(39,1,'Crema fresca (Guate)','2','Desayuno','2026-04-25','09:10:00',5,'Cucharada'),
(40,1,'Rib Eye','12','Almuerzo','2026-04-25','14:00:00',6,'Onzas'),
(41,1,'Elote asado (unidad)','1','Almuerzo','2026-04-25','14:20:00',3,'Unidad'),
(42,1,'Guacamole','3','Almuerzo','2026-04-25','14:25:00',5,'Cucharada'),
(43,1,'Vino Tinto / Blanco (copa)','1','Cena','2026-04-25','20:00:00',3,'Unidad'),
(84,1,'Leche Entera','250','Refaccion','2026-04-20','10:30:00',2,'Mililitros'),
(85,1,'Jugo de Naranja natural (taza)','1','Desayuno','2026-04-20','08:20:00',7,'Vaso'),
(86,1,'Güisquil cocido (porción)','1','Cena','2026-04-20','19:15:00',8,'Porcion'),
(87,1,'Agua Pura','500','Refaccion','2026-04-21','11:00:00',2,'Mililitros'),
(88,1,'Gaseosa / Soda (lata)','1','Almuerzo','2026-04-21','13:50:00',7,'Vaso'),
(89,1,'Pastrami','2','Cena','2026-04-21','20:00:00',8,'Porcion'),
(90,1,'Yogur Natural (sin azúcar)','200','Desayuno','2026-04-22','07:45:00',2,'Mililitros'),
(91,1,'Cerveza Clara (lata/botella)','1','Cena','2026-04-22','21:00:00',7,'Vaso'),
(92,1,'Lasagna de carne (porción)','1','Almuerzo','2026-04-22','13:00:00',8,'Porcion'),
(93,1,'Gelatina (preparada)','1','Refaccion','2026-04-22','16:30:00',7,'Vaso'),
(94,1,'Leche Descremada (0%)','300','Desayuno','2026-04-23','07:30:00',2,'Mililitros'),
(95,1,'Vino Tinto / Blanco (copa)','1','Cena','2026-04-23','20:30:00',7,'Vaso'),
(96,1,'Ensalada de Frutas','1','Refaccion','2026-04-23','10:00:00',8,'Porcion'),
(97,1,'Atol de Elote (taza)','1','Refaccion','2026-04-24','17:00:00',7,'Vaso'),
(98,1,'Sopa de Mariscos (Sólidos)','1','Almuerzo','2026-04-24','13:15:00',8,'Porcion'),
(99,1,'Té natural (sin azúcar)','250','Cena','2026-04-24','21:00:00',2,'Mililitros'),
(100,1,'Incaparina preparada (taza)','1','Desayuno','2026-04-25','08:30:00',7,'Vaso'),
(101,1,'Puré de Papa (con leche)','1','Almuerzo','2026-04-25','13:40:00',8,'Porcion'),
(102,1,'Batido de Proteína','400','Refaccion','2026-04-25','18:00:00',2,'Mililitros'),
(103,1,'Arroz con Leche (taza)','1','Cena','2026-04-25','20:15:00',8,'Porcion'),
(104,1,'Gaseosa / Soda (lata)','2','Almuerzo','2026-04-25','01:01:24',3,'Unidad'),
(105,1,'Chorizo Colorado','3','Desayuno','2026-04-25','10:01:01',3,'Unidad'),
(106,5,'Papaya picada','2','Desayuno','2026-05-02','11:09:56',4,'Taza'),
(107,6,'Hot Dog con todo (Shuco)','2','Refaccion','2026-05-02','11:23:41',3,'Unidad');
/*!40000 ALTER TABLE `registros_comida` ENABLE KEYS */;
UNLOCK TABLES;
COMMIT;
SET AUTOCOMMIT=@OLD_AUTOCOMMIT;

--
-- Table structure for table `unidades_medida`
--

DROP TABLE IF EXISTS `unidades_medida`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `unidades_medida` (
  `id_unidad` int(11) NOT NULL AUTO_INCREMENT,
  `nombre_unidad` varchar(20) NOT NULL,
  PRIMARY KEY (`id_unidad`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unidades_medida`
--

SET @OLD_AUTOCOMMIT=@@AUTOCOMMIT, @@AUTOCOMMIT=0;
LOCK TABLES `unidades_medida` WRITE;
/*!40000 ALTER TABLE `unidades_medida` DISABLE KEYS */;
INSERT INTO `unidades_medida` VALUES
(1,'Gramos'),
(2,'Mililitros'),
(3,'Unidad'),
(4,'Taza'),
(5,'Cucharada'),
(6,'Onzas'),
(7,'Vaso'),
(8,'Porcion');
/*!40000 ALTER TABLE `unidades_medida` ENABLE KEYS */;
UNLOCK TABLES;
COMMIT;
SET AUTOCOMMIT=@OLD_AUTOCOMMIT;

--
-- Table structure for table `usuarios`
--

DROP TABLE IF EXISTS `usuarios`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `usuarios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre_completo` varchar(100) NOT NULL,
  `edad` int(11) NOT NULL,
  `genero` enum('Masculino','Femenino','Otro') NOT NULL,
  `departamento` varchar(100) DEFAULT NULL,
  `municipio` varchar(100) DEFAULT NULL,
  `peso` decimal(5,2) NOT NULL,
  `altura` decimal(3,2) NOT NULL,
  `actividad_fisica` enum('Sedentario','Ligera','Moderada','Intensa') NOT NULL,
  `ocupacion` varchar(100) DEFAULT NULL,
  `usuario` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `usuario` (`usuario`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `usuarios`
--

SET @OLD_AUTOCOMMIT=@@AUTOCOMMIT, @@AUTOCOMMIT=0;
LOCK TABLES `usuarios` WRITE;
/*!40000 ALTER TABLE `usuarios` DISABLE KEYS */;
INSERT INTO `usuarios` VALUES
(1,'JOSE GUZMAN',19,'Masculino','Chimaltenango','Chimaltenango',69.80,1.78,'Moderada','TECNICO INSTALADOR','JGUZMAN','3106'),
(2,'Alejandro Guzman',19,'Masculino','Chimaltenango','Chimaltenango',67.00,1.67,'Moderada','Estudiante','prosti67','prosti67'),
(3,'>Emilio Maarroquin',24,'Masculino','Quetzaltenango','Cabecera',89.00,1.79,'Moderada','Tecnico','EmilioMJ','emilio12345'),
(4,'F',20,'Masculino','Chimaltenango','Chimaltenango',90.00,1.75,'Moderada','TECNICO','MILTON5','55555'),
(5,'MILTO EFREN GUZMAN LOPEZ',19,'Masculino','Chimaltenango','Chimaltenango',73.00,1.80,'Intensa','BODEGUERO','MILTON12','12345'),
(6,'MYNOR PAR',32,'Masculino','Chimaltenango','Chimaltenango',88.00,1.62,'Ligera','ESTUDIANTE','JAVIER','123');
/*!40000 ALTER TABLE `usuarios` ENABLE KEYS */;
UNLOCK TABLES;
COMMIT;
SET AUTOCOMMIT=@OLD_AUTOCOMMIT;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*M!100616 SET NOTE_VERBOSITY=@OLD_NOTE_VERBOSITY */;

-- Dump completed on 2026-05-02 11:33:57
