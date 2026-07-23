SELECT  statuse.wanr, vert.waid, r.rohranfang, r.rohrende, r.laenge as Rohrlaenge, w.anzahl as AnzahlRohre, ele.id, bau.Deutsch,  ele.laenge, ele.winkel, ele.makeanlage
 FROM
 waxserver.werkauftrag w,
 waxserver.element ele,
 waxserver.rohrleitung r,
 waxserver.verteiler vert,
 waxstamm.bauelementgruppen bau,
 waxserver.status statuse
 where
 statuse.waid = w.waid 
 and
 ele.waid = w.waid
 and
 vert.waid=w.waid
 and 
 r.waid= w.waid
 and   bau.elemid = ele.id
 and  vert.waid in
(SELECT distinct s.waid FROM waxserver.status s
where
 s.storniert=0
 and   s.freigabedatum >= *WaxDateVon*
 and   s.freigabedatum <= *WaxDateBis*
 and  s.waid in 
 (SELECT distinct FertSaege.waid FROM waxserver.FertSaegeNutRohre FertSaege))
