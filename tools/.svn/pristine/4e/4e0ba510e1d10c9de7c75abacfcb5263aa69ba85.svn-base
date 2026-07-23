SELECT s.wanr,w.waid,w.sdauftragsnr, w.sdpositionsnr,w.sdmaterialnr,w.posnr, w.anzahl,
       s.freigabedatum  as Freigabedatum,
       w.classid,w.auftragsbez,w.bezeichnung,w.sachbearbeiternl,
       s.storniert

FROM waxserver.werkauftrag w,
     waxserver.status s
where w.waid=s.waid
and w.sdauftragsnr = *SdAuftrag*
and w.sdpositionsnr >= *SdPosVon*
and w.sdpositionsnr <= *SdPosBis*
order by w.sdpositionsnr, w.waid