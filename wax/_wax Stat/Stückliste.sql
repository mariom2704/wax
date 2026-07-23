SELECT s.elemid as ElementId,s.Artikelnr,sap.materialbe as Bezeichnung,s.DN,s.AnschlussDN,sum(s.anzahllaenge)as Menge,sap.me as Einheit
FROM waxserver.status st,
     waxserver.stuecklistewa s
LEFT JOIN waxserver.sapmaterialstamm sap ON s.artikelnr=sap.materialnu
where s.waid=st.waid
and   st.storniert=0
and   st.freigabedatum >= *WaxDateVon*
and   st.freigabedatum <= *WaxDateBis*
group by s.elemid,s.artikelnr, s.anschlussdn
order by s.elemid,s.dn,s.anschlussdn,s.artikelnr
