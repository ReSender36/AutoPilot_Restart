
/*
SELECT l.event_num, se.caption,max(l.tran_date) event_moment
  FROM logs l
  INNER JOIN Sys_Events se ON se.kind = l.event_num
  WHERE l.event_num IN(157,158)
  GROUP BY l.event_num, se.caption
  ORDER BY max(l.tran_date)
*/
/*
  SELECT *
  FROM logs
  WHERE event_num IN(157,158,164,161,162,163,165,166)
*/
/*
SELECT d.IDDOC, COUNT(d.SP2779) 
  FROM DH465 d
  WHERE d.SP2779 = 1322112
  GROUP BY d.IDDOC
--  HAVING COUNT(d.SP2779) > 1
  */
  SELECT *
  FROM logs
  WHERE event_num IN(159)

/*
SELECT *
  FROM Sys_Events se
  WHERE se.kind_SubSystem = 7
  */
/*
SELECT l.event_num AS lastEvent
  FROM logs l
  WHERE l.event_num in(157,158)
  AND l.tran_date > (
    SELECT max(l.tran_date)
    FROM logs l
    WHERE l.event_num IN(158)
--    GROUP BY l.event_num
  )
*/
--  SELECT event_num AS lastEvent FROM DB1S_monitor.dbo.LOGS\t\tWHERE event_num in(157,158) \t\tAND tran_date > (\t\t\tSELECT max(tran_date) FROM LOGS\t\t\tWHERE l.event_num event_num = 158
--  SELECT event_num AS lastEvent FROM DB1S_monitor.dbo.LOGS WHERE event_num in(157,158) \t\tAND tran_date > (\t\t\tSELECT max(tran_date) FROM LOGS\t\t\tWHERE l.event_num event_num = 158
  -- SELECT event_num AS lastEvent FROM DB1S_monitor.dbo.LOGS WHERE event_num in(157,158) AND tran_date > (SELECT max(tran_date) FROM LOGS WHERE event_num = 158)
 -- SELECT event_num AS lastEvent FROM LOGS WHERE event_num in(157,158) AND tran_date > (SELECT max(tran_date) FROM LOGS WHERE event_num = 158)
  
 /* 
SELECT *
  FROM Sys_Events se
  WHERE se.caption LIKE '%автоп%'
*/