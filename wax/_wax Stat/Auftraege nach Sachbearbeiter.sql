SELECT sum(w.Anzahl) as GesPositionen, count(w.posnr) as AnzDiffPositionen, w.sachbearbeiternl, w.nl, n.bezeichnung, p.datei, st.freigabedatum  as Freigabedatum
FROM waxserver.werkauftrag w,
     waxstamm.niederlassungen n, 
     waxserver.status st
LEFT JOIN waxserver.Position p ON p.Waid = st.waid
where    w.waid=st.waid
and   n.nlnummer = w.nl
and   st.freigabedatum >= *WaxDateVon*
and   st.freigabedatum <= *WaxDateBis*
and  st.storniert = 0
and w.nl >= 281 and w.nl <= 299
and w.nl != 284

group by w.sachbearbeiternl, p.datei
order by n.region, w.nl, w.sachbearbeiternl