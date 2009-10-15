MAINTAINERCLEANFILES = Makefile.in

# FIX THIS, the subdirs should be set on AM_COND
SUBDIRS = \
mp25xxf \
mp2520f \
mp2530f \
dm320 \
dm6446

noinst_LTLIBRARIES = libequanime_drivers.la

libequanime_drivers_la_SOURCES =

libequanime_drivers_la_DEPENDENCIES = \
${top_builddir}/src/lib/drivers/mp25xxf/libequanime_mp25xxf.la \
${top_builddir}/src/lib/drivers/mp2520f/libequanime_mp2520f.la \
${top_builddir}/src/lib/drivers/mp2530f/libequanime_mp2530f.la \
${top_builddir}/src/lib/drivers/dm320/libequanime_dm320.la \
${top_builddir}/src/lib/drivers/dm6446/libequanime_dm6446.la

libequanime_drivers_la_LIBADD = \
${top_builddir}/src/lib/drivers/mp25xxf/libequanime_mp25xxf.la \
${top_builddir}/src/lib/drivers/mp2520f/libequanime_mp2520f.la \
${top_builddir}/src/lib/drivers/mp2530f/libequanime_mp2530f.la \
${top_builddir}/src/lib/drivers/dm320/libequanime_dm320.la \
${top_builddir}/src/lib/drivers/dm6446/libequanime_dm6446.la
