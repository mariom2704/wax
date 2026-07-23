SELECT r.wanr, l.waid, l.counter, l.posnr, l.elementid, l.elementdn, l.absabst, l.winkel
FROM waxserver.FertSaegeNutRohre r,
waxserver.FertVerteilerNutAbgang l
where r.waid=l.waid

and r.wanr=*Wanr*

group by wanr, r.waid, r.teilrohrcounter, l.counter
order by r.wanr, r.waid, r.sortierung, l.counter