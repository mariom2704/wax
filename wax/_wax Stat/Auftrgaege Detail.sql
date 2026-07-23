SELECT w.Waid, w.sdauftragsnr, w.posnr, st.wanr, w.Anzahl, w.sachbearbeiternl, w.nl, n.Bezeichnung, st.freigabedatum  as Freigabedatum, p.Datei, p.Verzeichnis, st.status, f.bez, st.sachbesegment as BearbeiterOD, st.storniert_von
FROM waxserver.werkauftrag w,
     waxstamm.niederlassungen n,
     waxserver.freigabestatus f,
     waxserver.status st
LEFT JOIN waxserver.Position p ON p.Waid = st.waid
where    w.waid=st.waid
and   n.nlnummer = w.nl
and   st.status = f.id
and w.nl >= 281 and w.nl <= 299
and w.nl != 284
and   st.freigabedatum >= *WaxDateVon*
and   st.freigabedatum <= *WaxDateBis*
