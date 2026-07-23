SELECT st.wanr,w.sdauftragsnr, w.sdpositionsnr,w.sdmaterialnr,
       s.elemid as ElementId,s.Artikelnr,sap.materialbe as Bezeichnung,
       s.DN,s.AnschlussDN,sum(s.anzahllaenge)as Menge,sap.me as Einheit
FROM waxserver.werkauftrag w,
     waxserver.status st,
     waxserver.stuecklistewa s
LEFT JOIN waxserver.sapmaterialstamm sap ON s.artikelnr=sap.materialnu
where w.waid=st.waid
and   s.waid=st.waid
and   st.storniert=0
and w.sdauftragsnr = *SdAuftrag*
and w.sdpositionsnr >= *SdPosVon*
and w.sdpositionsnr <= *SdPosBis*
group by st.wanr,s.elemid,s.artikelnr, s.anschlussdn
order by st.wanr,s.elemid,s.dn,s.anschlussdn,s.artikelnr
