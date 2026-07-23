SELECT sum(w.Anzahl) as GesPositionen,
       IFNULL(sum(if (LENGTH(p.datei) > 0, w.Anzahl, NULL)) / sum(w.Anzahl) * 100, 0) as NutzungGesPosRnk,
       count(w.posnr) as AnzDiffPositionen,
       count(if (LENGTH(p.datei) > 0, w.posnr, NULL)) / count(w.posnr) * 100 as NutzungDiffPosRnk,
       w.nl
FROM waxserver.werkauftrag w,     
     waxserver.status st
LEFT JOIN waxserver.Position p ON p.Waid = st.waid
where    w.waid=st.waid
and   st.freigabedatum >= *WaxDateVon*
and   st.freigabedatum <= *WaxDateBis*
and  st.storniert = 0
group by w.nl
order by w.nl