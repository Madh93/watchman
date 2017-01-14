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
DAEMON_OPTS=""
PIDFILE=/var/run/$NAME.pid

# Exit if the package is not installed
[ -x "$DAEMON" ] || exit 0

# Read configuration variable file if it is present
[ -r /etc/default/$NAME.conf ] && . /etc/default/$NAME.conf
[ -r /etc/$NAME.conf ] && . /etc/$NAME.conf

is_running() {
  pid=$(cat $PIDFILE)
  if [ -n "$(ps -A | grep $pid)" ]; then
    return 0
  else
    return 1
  fi
}

start() {
  if [[ -f "$PIDFILE" ]] && is_running ; then
      echo "$NAME is already running"
      return 1
  fi

  echo "Starting $NAME..."
  $DAEMON && pgrep -f $DAEMON > $PIDFILE
  echo "$NAME started"
}

stop() {
  if [[ ! -f "$PIDFILE" ]] || ! is_running ; then
    echo "$NAME is stopped"
    return 1
  fi

  echo "Stopping $NAME..."
  pkill -F $PIDFILE && rm -f $PIDFILE
  echo "$NAME stopped"
}

status() {
  if [[ ! -f "$PIDFILE" ]] || ! is_running ; then
    echo "$NAME is stopped"
  else
    echo "$NAME is running"
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
