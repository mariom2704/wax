SELECT s.wanr, w.waid, st.muffenDN as Muffen_DN,
       (sign(st.muffenrelabst1) + sign(st.muffenrelabst2) + sign(st.muffenrelabst3) + sign(st.muffenrelabst4) +
       sign(st.muffenrelabst5) + sign(st.muffenrelabst6) + sign(st.muffenrelabst7) + sign(st.muffenrelabst8) +
       sign(st.muffenrelabst9) + sign(st.muffenrelabst10)) as Anz_Muffen_pro_Rohr,
       (sign(st.muffenrelabst1) + sign(st.muffenrelabst2) + sign(st.muffenrelabst3) + sign(st.muffenrelabst4) +
       sign(st.muffenrelabst5) + sign(st.muffenrelabst6) + sign(st.muffenrelabst7) + sign(st.muffenrelabst8) +
       sign(st.muffenrelabst9) + sign(st.muffenrelabst10)) * w.anzahl AS Anzahl_Muffen, w.anzahl AS Anzahl_Rohre
       , w.anzahl, 0 as UngleichSechsMeter
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

UNION
SELECT s.wanr, s.waid, 0, 0, r.anzahl, 0, 0,  r.laenge 
FROM waxserver.rohrleitung r,
     waxserver.status s,
     waxserver.strangrohr st
where  s.waid=r.waid
and s.waid=st.waid
and   s.storniert=0
and s.wanr > 0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and r.laenge <> 6000 order by wanr