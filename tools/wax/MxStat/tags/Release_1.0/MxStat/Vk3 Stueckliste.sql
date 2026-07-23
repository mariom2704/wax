SELECT s.eleid as ElementId,s.Artikelnr,vk.bezeichnung_deutsch as Bezeichnung,s.DN,s.AnschlussDN,sum(s.anzahllaenge)as Menge,sap.me as Einheit, sum(s.anzahllaenge * s.preis) as Preis
FROM waxserver.status st,
     waxserver.vk3listewa s
LEFT JOIN waxserver.sapmaterialstamm sap ON s.artikelnr=sap.materialnu
LEFT JOIN waxstamm.bauelementgruppen vk ON s.eleid=vk.elemid
where s.waid=st.waid
and   st.storniert=0
and   s.isvk3abr=1
and   st.freigabedatum >= *WaxDateVon*
and   st.freigabedatum <= *WaxDateBis*
group by s.eleid,s.artikelnr, s.anschlussdn
order by s.eleid,s.dn,s.anschlussdn,s.artikelnr



