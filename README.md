# Watchman

Simple implementation of inotify.

### To do

- [x] Lanzarse y detenerse correctamente.
- [x] Hacer el seguimiento de una o varias carpetas que se le pasen como parámetro.
- [x] Mostrar como mínimo todos los eventos se produzcan correspondientes en la tabla colgada en el campusvirtual EventosInotify al primer bloque (de IN_ACCESS a IN_OPEN).
- [x] **Opcional:** No volver a lanzar el demonio si ya está activo.
- [x] **Opcional:** Monitorizar los eventos en todos los subdirectorios de los directorios especificados.
- [x] **Opcional:** Si se añaden o borran subdirectorios la monitorización debería actualizarse dinámicamente.
