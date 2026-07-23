SELECT s.wanr, a.waid, a.txt, a.anzahl, a.zeit, a.ruestzeit, a.preis
FROM waxserver.werkauftrag w,
     waxserver.status s,
     waxserver.rohrleitung r,
     waxserver.arbeit a
where   w.waid=s.waid
and   w.waid=a.waid
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and   w.waid=r.waid
and   s.storniert=0
and   (a.id = 55)
