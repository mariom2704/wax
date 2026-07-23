SELECT w.waid,w.sdauftragsnr, w.sdpositionsnr,w.sdmaterialnr,w.posnr,w.anzahl,
       w.classid,w.auftragsbez,w.bezeichnung,w.sachbearbeiternl
FROM waxserver.werkauftrag w

where  w.sdauftragsnr = *SdAuftrag*
and w.sdpositionsnr >= *SdPosVon*
and w.sdpositionsnr <= *SdPosBis*

order by w.sdpositionsnr, w.waid