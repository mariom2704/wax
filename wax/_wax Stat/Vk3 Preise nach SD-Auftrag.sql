SELECT s.wanr,w.sdauftragsnr, w.sdpositionsnr,w.sdmaterialnr, sa.preis,
       s.freigabedatum  as Freigabedatum,
       w.auftragsbez,w.bezeichnung,w.sachbearbeiternl

FROM waxserver.werkauftrag w,
     waxserver.status s,
     waxserver.sap sa

where w.waid=s.waid
and s.wanr=sa.wanr
and w.sdauftragsnr = *SdAuftrag*
and w.sdpositionsnr >= *SdPosVon*
and w.sdpositionsnr <= *SdPosBis*
group by s.wanr
order by w.sdpositionsnr