#! /bin/bash

### BEGIN INIT INFO
# Provides:          watchman
# Required-Start:    $network $local_fs $remote_fs
# Required-Stop:     $network $local_fs $remote_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Description:       Simple implementation of inotify
### END INIT INFO

# SETTINGS
NAME=watchman
SCRIPT=/usr/local/bin/$NAME
PIDFILE=/var/run/$NAME.pid
LOGFILE=/var/log/$NAME.pid

start() {
  if [ -e $PIDFILE ]; then
    echo "$NAME is already running"
    return 1
  fi
  echo "Starting $NAME..."
  $SCRIPT
  echo "$NAME started"
}

stop() {
  if [ ! -e "$PIDFILE" ]; then
    echo "$NAME is not running"
    return 1
  fi
  echo "Stopping $NAME..."
  pkill -F $PIDFILE
  echo "$NAME stopped"
}

status() {
  if [ -e $PIDFILE ]; then
    echo "$NAME is running"
  else
    echo "$NAME is not running"
  fi
}

case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  status)
    status
    ;;
  retart)
    stop
    start
    ;;
  *)
    echo "Usage: $0 {start|stop|status|restart}"
esac
