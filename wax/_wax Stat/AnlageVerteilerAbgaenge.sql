SELECT f.waid, f.counter, f.posnr, f.dn, f.laenge, f.anfang, f.ende, f.elementid, f.elementdn, f.absabst, f.winkel, f.elementlaenge
FROM waxserver.FertVerteilerNutAbgang f,
waxserver.status s
where s.waid=f.waid
and s.wanr=*Wanr*
order by f.posnr, f.counter