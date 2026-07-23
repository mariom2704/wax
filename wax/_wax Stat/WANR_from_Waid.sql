SELECT s.wanr,w.waid,w.sdauftragsnr, w.sdpositionsnr,w.sdmaterialnr,w.posnr, w.anzahl,
       s.freigabedatum  as Freigabedatum,
       w.classid,w.auftragsbez,w.bezeichnung,w.sachbearbeiternl, s.sachbesegment,
       s.storniert

FROM waxserver.werkauftrag w,
     waxserver.status s
where w.waid=s.waid
and w.waid = *waid*
order by w.sdpositionsnr, w.waid