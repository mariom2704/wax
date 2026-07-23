SELECT a.* FROM waxserver.status s,waxserver.arbeit a where a.waid=s.waid and s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis* and a.id = 15;