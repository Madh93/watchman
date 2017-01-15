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
DAEMON=/usr/local/bin/$NAME
PIDFILE=/var/run/$NAME.pid

# Exit if the package is not installed
[ -x "$DAEMON" ] || exit 0

# Read configuration variable file if it is present
[ -r /etc/default/$NAME.conf ] && . /etc/default/$NAME.conf
[ -r /etc/$NAME.conf ] && . /etc/$NAME.conf

is_running() {
  [ -f "$PIDFILE" ] || return 1   # $PIDFILE exists
  [ -s "$PIDFILE" ] || return 1   # $PIDFILE is not empty

  pid=$(cat $PIDFILE)
  if [ -n "$(ps -A | grep $pid)" ]; then
    return 0
  else
    return 1
  fi
}

start() {
  if is_running ; then
      echo "$NAME is already running"
      return 1
  fi

  echo "Starting $NAME..."
  $DAEMON $WATCHMAN_ARGS && pgrep -f $DAEMON > $PIDFILE
  echo "$NAME started"
}

stop() {
  if ! is_running ; then
    echo "$NAME is stopped"
    return 1
  fi

  echo "Stopping $NAME..."
  pkill -F $PIDFILE && rm -f $PIDFILE
  echo "$NAME stopped"
}

status() {
  if is_running ; then
    echo "$NAME is running"
  else
    echo "$NAME is stopped"
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
  restart)
    stop
    start
    ;;
  *)
    echo "Usage: $0 {start|stop|status|restart}"
esac
