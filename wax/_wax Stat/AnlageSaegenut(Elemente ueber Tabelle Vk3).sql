SELECT   statuse.wanr, vert.waid, r.rohranfang, r.rohrende, r.laenge as Rohrlaenge, w.anzahl as AnzahlRohre, vklist.dn, vklist.anschlussdn, vklist.eleid, bau.Deutsch, vklist.anzahllaenge AS AnzahlElemente
 FROM 
 waxserver.werkauftrag w,
 waxserver.vk3listewa vklist,
 
 waxserver.rohrleitung r,
 waxserver.verteiler vert,
 waxstamm.bauelementgruppen bau,
 waxserver.status statuse
 where 
 statuse.waid = w.waid 
 and
 w.waid= r.waid
 and   w.waid=vklist.waid
 and   w.waid=vert.waid
 and   bau.elemid = vklist.eleid
 and
  vert.waid in
(SELECT distinct s.waid FROM waxserver.status s
where
 s.storniert=0
and   s.freigabedatum >= *WaxDateVon*
 and   s.freigabedatum <= *WaxDateBis*
 and  s.waid in 
 (SELECT distinct FertSaege.waid FROM waxserver.FertSaegeNutRohre FertSaege))