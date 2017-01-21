# Watchman

Simple implementation of inotify.

### To do

- [x] Lanzarse y detenerse correctamente.
- [x] Hacer el seguimiento de una o varias carpetas que se le pasen como parámetro.
- [x] Mostrar como mínimo todos los eventos se produzcan correspondientes en la tabla colgada en el campusvirtual EventosInotify al primer bloque (de IN_ACCESS a IN_OPEN).
- [x] **Opcional:** No volver a lanzar el demonio si ya está activo.
- [x] **Opcional:** Monitorizar los eventos en todos los subdirectorios de los directorios especificados.
- [x] **Opcional:** Si se añaden o borran subdirectorios la monitorización debería actualizarse dinámicamente.

### Installation

    git clone https://github.com/Madh93/watchman && cd watchman
    make
    sudo make install

### Usage

    watchman [options] DIRECTORIES

Start service using default settings:

    service watchman start

Defaults settings are available in `/etc/default/watchman.conf`:

    WATCHMAN_ARGS="--host localhost --port 12345 /vagrant_data/watchman/test"

You can edit or override arguments:

    service watchman start --port 7635 /path_to_dir /path_to_other_dir

(If no host or port is passed `watchman` set up defaults: `localhost:12345`).

##### Logs

Logs are available in `/var/log/watchman.log`:

    tail -f /var/log/watchman.log

    Jan 21 09:55:10 watchman[5015]: Starting...
    Jan 21 09:55:10 watchman[5015]: Started watchman
    Jan 21 09:55:10 watchman[5015]: Started monitor
    Jan 21 09:55:10 watchman[5015]: Started connection
    Jan 21 09:55:10 watchman[5015]: Server says: Welcome!
    Jan 21 09:55:10 watchman[5015]: Added watch '/vagrant_data/watchman/test/beatles' (wd: 1)
    Jan 21 09:55:10 watchman[5015]: Added watch '/vagrant_data/watchman/test' (wd: 2)
    Jan 21 09:55:47 watchman[5015]: IN_DELETE: file 'top.txt' deleted in '/vagrant_data/watchman/test' (wd: 2)
    Jan 21 09:56:13 watchman[5015]: IN_CREATE: directory 'rolling' created in '/vagrant_data/watchman/test' (wd: 2)
    Jan 21 09:56:13 watchman[5015]: Added watch '/vagrant_data/watchman/test/rolling' (wd: 3)
    Jan 21 09:57:36 watchman[5015]: Received SIGTERM
    Jan 21 09:57:36 watchman[5015]: Connection closed
    Jan 21 09:57:36 watchman[5015]: Removed watch '/vagrant_data/watchman/test/rolling' (wd: 3)
    Jan 21 09:57:36 watchman[5015]: Removed watch '/vagrant_data/watchman/test/beatles' (wd: 1)
    Jan 21 09:57:36 watchman[5015]: Removed watch '/vagrant_data/watchman/test' (wd: 2)
    Jan 21 09:57:36 watchman[5015]: Monitor closed
    Jan 21 09:57:36 watchman[5015]: Finished watchman
