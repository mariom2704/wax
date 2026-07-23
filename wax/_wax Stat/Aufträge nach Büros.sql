select w.nl, n.bezeichnung, count(w.waid) as AnzDiffPos, sum(w.Anzahl) as AnzPos
FROM waxserver.werkauftrag w,
     waxstamm.niederlassungen n,
     waxserver.status st
where    w.waid=st.waid
and   n.nlnummer = w.nl
and   st.freigabedatum >= *WaxDateVon*
and   st.freigabedatum <= *WaxDateBis*
group by w.nl
order by w.nl