SELECT s.wanr, w.waid, st.muffenDN as Muffen_DN,
       (sign(st.muffenrelabst1) + sign(st.muffenrelabst2) + sign(st.muffenrelabst3) + sign(st.muffenrelabst4) +
       sign(st.muffenrelabst5) + sign(st.muffenrelabst6) + sign(st.muffenrelabst7) + sign(st.muffenrelabst8) +
       sign(st.muffenrelabst9) + sign(st.muffenrelabst10)) as Anz_Muffen_pro_Rohr,
       (sign(st.muffenrelabst1) + sign(st.muffenrelabst2) + sign(st.muffenrelabst3) + sign(st.muffenrelabst4) +
       sign(st.muffenrelabst5) + sign(st.muffenrelabst6) + sign(st.muffenrelabst7) + sign(st.muffenrelabst8) +
       sign(st.muffenrelabst9) + sign(st.muffenrelabst10)) * w.anzahl AS Anzahl_Muffen, w.anzahl AS Anzahl_Rohre
       , w.anzahl, r.laenge
FROM waxserver.werkauftrag w,
     waxserver.strangrohr st,
     waxserver.status s,
     waxserver.rohrleitung r,
     waxstamm.bauelementgruppen b
where w.waid=st.waid
and   w.waid=s.waid
and   w.waid=r.waid
and   st.muffenID=b.elemid
and   s.storniert=0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and   (st.muffenID = 5 OR st.muffenID = 6 OR st.muffenID = 7 OR  st.muffenID = 24 OR  st.muffenID = 59)