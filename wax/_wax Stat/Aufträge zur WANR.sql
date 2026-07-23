SELECT s.wanr,w.waid,w.sdauftragsnr, w.sdpositionsnr,w.sdmaterialnr,w.posnr, w.anzahl,
       s.freigabedatum  as Freigabedatum,
       w.classid,w.auftragsbez,w.bezeichnung,w.sachbearbeiternl, s.sachbesegment, 
       s.status,f.bez,s.storniert, s.storniert_von, p.Datei, p.Verzeichnis

FROM waxserver.werkauftrag w,     
     waxserver.freigabestatus f,
     waxserver.status s
     LEFT JOIN waxserver.Position p ON p.Waid = s.waid

where w.waid=s.waid
and s.status=f.id
and s.wanr = *Wanr*
order by w.sdpositionsnr, w.waid