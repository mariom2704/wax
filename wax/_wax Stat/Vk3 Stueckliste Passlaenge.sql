SELECT st.wanr, s.eleid as ElementId,s.Artikelnr,vk.Deutsch as Bezeichnung,s.DN,s.AnschlussDN,sum(s.anzahllaenge)as Menge, sum(s.anzahllaenge * 
s.preis) as Preis
FROM waxserver.status st,
     waxserver.vk3listewa s
LEFT JOIN waxstamm.bauelementgruppen vk ON s.eleid=vk.elemid
where s.waid=st.waid
and   st.storniert=0
and   s.isvk3abr=1
and   st.freigabedatum >= *WaxDateVon*
and   st.freigabedatum <= *WaxDateBis*
and   s.eleid >= 551 and s.eleid <= 556
group by s.eleid,s.artikelnr, s.anschlussdn
order by s.eleid,s.dn,s.anschlussdn,s.artikelnr


