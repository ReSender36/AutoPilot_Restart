-- insert INTO RESTART_EVENTS(RESTART_USER_ID, STATE) values(12, 1)
-- SELECT row_id FROM Name_Mail nm WHERE nm.EMP_LOGIN = 'автопилот'
-- select REST_ID FROM RESTART_EVENTS WHERE STATE = 1

SELECT l.RESTART_EVENT,
  MAX(CASE WHEN l.event_num = 161 THEN l.tran_date ELSE '' END) init_proc,
  MAX(CASE WHEN l.event_num = 158 THEN l.tran_date ELSE '' END) stop_auto,
  MAX(CASE WHEN l.event_num = 163 THEN l.tran_date ELSE '' END) end_timer,
  MAX(CASE WHEN l.event_num = 157 THEN l.tran_date ELSE '' END) start_auto,
  MAX(CASE WHEN l.event_num = 168 THEN l.tran_date ELSE '' END) close_restart_session,
  MAX(CASE WHEN l.event_num = 167 THEN l.tran_date ELSE '' END) delay_timer,
  MAX(CASE WHEN l.event_num = 166 THEN l.tran_date ELSE '' END) cancel_restart,
  MAX(CASE WHEN l.event_num = 165 THEN l.tran_date ELSE '' END) cancel_restart_run_auto
  FROM logs l
  WHERE l.tran_date BETWEEN '22.04.2017 00:00:00' AND '27.04.2017 23:59:59'
  AND l.event_num IN(157,158,164,161,162,163,165,166,168)
  GROUP BY l.RESTART_EVENT