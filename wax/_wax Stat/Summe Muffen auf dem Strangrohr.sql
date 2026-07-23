SELECT s.wanr,st.muffenDN as Muffen_DN,
       sum(w.anzahl * (sign(st.muffenrelabst1) + sign(st.muffenrelabst2) + sign(st.muffenrelabst3) + sign(st.muffenrelabst4) +
       sign(st.muffenrelabst5) + sign(st.muffenrelabst6) + sign(st.muffenrelabst7) + sign(st.muffenrelabst8) +
       sign(st.muffenrelabst9) + sign(st.muffenrelabst10))) as Anz_Muffen,
       b.Deutsch as Bezeichnung, s.freigabedatum as Freigabedatum, w.pspelement, w.auftragsbez
FROM waxserver.werkauftrag w,
     waxserver.strangrohr st,
     waxserver.status s,
     waxstamm.bauelementgruppen b
where w.waid=st.waid
and   w.waid=s.waid
and   st.muffenID=b.elemid
and   s.storniert=0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
order by Anz_Muffen desc