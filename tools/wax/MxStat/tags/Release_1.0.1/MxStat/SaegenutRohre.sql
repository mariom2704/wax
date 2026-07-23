SELECT r.Wanr, r.rohrcounter, r.teilrohrcounter, r.waid, d.posnr, d.dn, d.laenge, d.nutanfang, d.nutende, d.anzloecher, d.ablageplatz
FROM waxserver.FertSaegeNutRohre r,
waxserver.FertSaegeNutRohreDaten d
where r.waid=d.waid
and r.wanr=*Wanr*
group by wanr, r.waid, r.teilrohrcounter
order by r.wanr, r.sortierung