##### Coronazuil groep 4 ####
##### DATA-ANALYSE ####


##### libraries importeren ####
library(Hmisc)
library(reshape2)
library(Hmisc)
library(car)
library(effects)
library(BayesFactor)
library(lsr)
library(tidyr)
library(reshape)
library(lme4)
library(afex)
library(psy)
library(dplyr)
library(readr)

##### RUWE DATA INLEZEN #####

Data<-read.table("~/Downloads/data_analyse_groep4/raw_data_coronazuil_groep4.csv", header=TRUE, sep=',')
#View(Data)
    ##### Hiermee krijgen we een eerste beeld van de data (bv. werd alle data correct ingelezen?) #####
describe(Data) 


##### RUWE DATA OPKUISEN ####

    ##### Kwalitatieve data verwijderen (met ongestructureerde data kunnen we niet rekenen) #####
    ##### We verwijderen een aantal kolommen #####
Data <- Data[,c(2:6,9:14)]

    ##### We voegen een kolom met participantnr. toe #####
Data$participant <- 1:nrow(Data)

    ##### We geven de kolommen een kortere naam zodat we er makkelijker naar kunnen verwijzen#####
      ##### Hoe ziet het er nu uit? #####
      colnames(Data)

Data <- Data %>% rename(leeftijdscategorie = Wat.is.jouw.leeftijdscategorie.)
Data <- Data %>% rename(passeren = Maak.een.schatting.van.hoeveel.keer.je.op.een.gemiddelde.dag.een.ontsmettingszuil.passeert..Typ.enkel.het.cijfer.)
Data <- Data %>% rename(gebruiken = Hoeveel.procent.van.de.keren.dat.je.een.ontsmettingszuil.passeerde..heb.je.deze.ook.effectief.gebruikt.gedurende.de.afgelopen.twee.weken..Een.ruwe.schatting.volstaat.)
Data <- Data %>% rename(belang_wrijven = Hoe.belangrijk.is.het.volgens.jou.om.grondig.en.langdurig.de.handen.over.elkaar.te.bewegen.wanneer.je.alcoholgel.gebruikt.)
Data <- Data %>% rename(belang_gebruiken = In.welke.mate.denk.je.dat.de.handen.ontsmetten.belangrijk.is.om.de.verspreiding.van.het.virus.tegen.te.gaan.)
Data <- Data %>% rename(type_gel = Welke.manier.van.ontsmetting.verkies.je.)
Data <- Data %>% rename(wrijfduur = Hoe.lang.moet.je.de.handen.over.elkaar.bewegen..denk.jij..om.80.tot.100...van.de.microben.en.virussen.te.doden.)
Data <- Data %>% rename(pict_bev = Welke.pictogrammen.geven.volgens.jou.het.duidelijkst.de.volgende.boodschap.weer...Plaats.de.handen.zodat.ze.bevochtigd.worden...Duid.minimum.....n.en.maximum.drie.pictogrammen.aan.)
Data <- Data %>% rename(pict_voetpomp = Welke.pictogrammen.geven.volgens.jou.het.duidelijkst.de.volgende.boodschap.weer...Gebruik.de.voetpomp...Duid.minimum.....n.en.maximum.drie.pictogrammen.aan.)
Data <- Data %>% rename(pict_wrijven = Welke.pictogrammen.geven.volgens.jou.het.duidelijkst.de.volgende.boodschap.weer...Beweeg.de.handen.over.elkaar...Duid.minimum.....n.en.maximum.twee.pictogrammen.aan.)
Data <- Data %>% rename(toekomst = Tot.slot..Denk.je.dat.de.handen.ontsmetten.ook.na.deze.epidemie.op.veel.plaatsen.verplicht.of.aanbevolen.zal.blijven..Bv..in.cinemazalen..de.bank..auditoria..de.bakker.enzovoort.)

      ##### Is het gelukt? #####
      colnames(Data)

    ##### Factors aanmaken
Data$gebruiken <- factor(Data$gebruiken, levels = c("0 - 20 %", "20 - 40 %", "40 - 60 %", "60 - 80 %", "80 - 100 %"))
Data$wrijfduur <- factor(Data$wrijfduur, levels = c("1 - 5 seconden", "5 - 10 seconden", "10 - 15 seconden", "15 - 20 seconden", "Langer dan 20 seconden"))
Data$leeftijdscategorie <- factor(Data$leeftijdscategorie, levels = c("Jonger dan 13 jaar", "13 - 18 jaar", "19 - 24 jaar", "25 - 30 jaar", "31 - 36 jaar", "36 - 41 jaar", "42 - 47 jaar", "48 - 53 jaar", "54 - 59 jaar", "60 - 65 jaar", "66 - 71 jaar", "72 - 77 jaar", "78 - 83 jaar"))
     ##### Numerieke vectors maken
Data$gebruiken <- as.numeric(Data$gebruiken)
Data$wrijfduur <- as.numeric(Data$wrijfduur)
Data$leeftijdscategorie <- as.numeric(Data$leeftijdscategorie)

    ##### Participanten met ongeldige data (5 personen; bv. 'veel' schrijven wanneer een cijfer werd gevraagd) verwijderen
    ##### Alle data van de pp wordt verwijderd om missing data ('NA') te voorkomen #####
    ##### parse_number smijt alles voor en na het eerste cijfer weg en noteert tekst als 'NA' #####
Data$passeren <- as.character(Data$passeren)
Data$passeren <- parse_number(Data$passeren, trim_ws = FALSE)
Data <- na.omit(Data)
describe(Data$participant)


##### DATA-ANALYSE #####

    ##### De antwoorden op twee 5-P-Likertschalen voor 'het belang van wrijven' en 'het belang van gebruiken' zijn significant gecorreleerd (p<.001)
    ##### maar het verband is eerder 'zwak' (rho = .38)
    ##### We gebruiken de Spearman correlatie omdat we werken met twee ordinale schalen
cor.test(Data$gebruiken, Data$belang_gebruiken, method="spearman")
cor.test(Data$gebruiken, Data$wrijfduur, method="spearman")
cor.test(Data$gebruiken, Data$leeftijdscategorie, method="spearman")

