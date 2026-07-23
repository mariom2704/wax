SELECT r.Wanr, r.rohrcounter, r.teilrohrcounter, r.sortierung, r.waid, count(r.teilrohrcounter) as Anzahl,
       d.posnr, d.dn, d.laenge, d.nutanfang, d.nutende, d.anzloecher, d.ablageplatz
FROM waxserver.FertSaegeNutRohre r,
     waxserver.FertSaegeNutRohreDaten d

where  r.waid=d.waid
and r.teilrohrcounter=d.teilrohrcounter

and wanr=*Wanr*

group by r.waid, r.teilrohrcounter
order by r.sortierung, r.waid, r.rohrcounter, r.teilrohrcounter