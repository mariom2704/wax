SELECT r.wanr, l.waid, l.teilrohrcounter, l.lochcounter, l.lochtyp, l.lochdn, l.abstand, l.winkel
FROM waxserver.FertSaegeNutRohre r,
waxserver.FertSaegeNutLoecher l
where r.waid=l.waid
and r.teilrohrcounter=l.teilrohrcounter
and r.wanr=*Wanr*
group by wanr, r.waid, r.teilrohrcounter, l.lochcounter
order by r.wanr, r.waid, r.sortierung