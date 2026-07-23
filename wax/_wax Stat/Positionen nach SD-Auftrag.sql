SELECT s.wanr,w.sdauftragsnr, w.sdpositionsnr,w.sdmaterialnr,w.posnr, w.anzahl,
       s.freigabedatum  as Freigabedatum,
       w.auftragsbez,w.bezeichnung,w.sachbearbeiternl, w.classid as Typ,
       s.status,f.bez as StatusBez,s.storniert, s.storniert_von, p.Datei, p.Verzeichnis

FROM waxserver.werkauftrag w,
     waxserver.freigabestatus f,
     waxserver.status s
     LEFT JOIN waxserver.Position p ON p.Waid = s.waid

where w.waid=s.waid
and s.status=f.id
and w.sdauftragsnr = *SdAuftrag*
and w.sdpositionsnr >= *SdPosVon*
and w.sdpositionsnr <= *SdPosBis*
order by w.sdpositionsnr, w.waid