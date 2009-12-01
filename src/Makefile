CPP       = g++
BIN       = ../stealth
#CPPFLAGS  = -Wall -I${INCDIR}
#LFLAGS    =  
CPPFLAGS  = -Wall -I${INCDIR}
LFLAGS    = 
INCDIR    = ../include

OBJS     = network.o\
	access.o\
	channel.o\
	chanclient.o\
	client.o\
	cmdmap.o\
	config.o\
	events.o\
	gline.o\
	jupe.o\
	main.o\
	match.o\
	msgmap.o\
	msgparse.o\
	server.o\
	signals.o\
	socket.o\
	stealth.o\
	timer.o\
	token.o\
	c_adduser.o\
	c_ban.o\
	c_broadcast.o\
	c_chaninfo.o\
	c_chanlist.o\
	c_chlevel.o\
	c_chname.o\
	c_clearchan.o\
	c_clone.o\
	c_cloneact.o\
	c_clonejoin.o\
	c_clonepart.o\
	c_clonequit.o\
	c_clonesay.o\
	c_ctcp.o\
	c_deop.o\
	c_deopall.o\
	c_devoice.o\
	c_devoiceall.o\
	c_die.o\
	c_disable.o\
	c_enable.o\
	c_gline.o\
	c_help.o\
	c_invite.o\
	c_inviteme.o\
	c_jupe.o\
	c_kick.o\
	c_login.o\
	c_logout.o\
	c_map.o\
	c_mode.o\
	c_moduser.o\
	c_op.o\
	c_opall.o\
	c_operinfo.o\
	c_opermsg.o\
	c_quote.o\
	c_refreshg.o\
	c_remgline.o\
	c_remuser.o\
	c_reop.o\
	c_restart.o\
	c_scan.o\
	c_servinfo.o\
	c_set.o\
	c_settime.o\
	c_show.o\
	c_showcommands.o\
	c_translate.o\
	c_unban.o\
	c_unjupe.o\
	c_uptime.o\
	c_voice.o\
	c_voiceall.o\
	c_whois.o\
	c_whoison.o\
	c_write.o\
	m_account.o\
	m_admin.o\
	m_away.o\
	m_burst.o\
	m_clearmode.o\
	m_create.o\
	m_destruct.o\
	m_desynch.o\
	m_eob.o\
	m_eoback.o\
	m_error.o\
	m_gline.o\
	m_invite.o\
	m_join.o\
	m_jupe.o\
	m_kick.o\
	m_kill.o\
	m_leave.o\
	m_mode.o\
	m_nick.o\
	m_pass.o\
	m_ping.o\
	m_privmsg.o\
	m_quit.o\
	m_server.o\
	m_silence.o\
	m_squit.o\
	m_stats.o\
	m_topic.o\
	m_version.o\
	m_wallops.o\
	m_wallusers.o\
	m_whois.o

all: ${BIN}

${BIN}: ${OBJS}
	${CPP} ${LFLAGS} -o ${BIN} ${OBJS}

clean: 
	@rm -rf ${OBJS} ${BIN} *~ .*.swp

#debug: CPPFLAGS  = -g3 -Wall -DDEBUG -DSOCKDEBUG -I${INCDIR}
#debug: LFLAGS    = -g3
debug: ${OBJS} ${BIN}

access.o: access.cc
	${CPP} ${CPPFLAGS} -o $@ -c access.cc

c_adduser.o: c_adduser.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_adduser.cc

c_ban.o: c_ban.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_ban.cc

c_broadcast.o: c_broadcast.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_broadcast.cc

c_chaninfo.o: c_chaninfo.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_chaninfo.cc

c_chanlist.o: c_chanlist.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_chanlist.cc

c_chlevel.o: c_chlevel.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_chlevel.cc

c_chname.o: c_chname.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_chname.cc

c_clearchan.o: c_clearchan.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_clearchan.cc

c_clone.o: c_clone.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_clone.cc

c_cloneact.o: c_cloneact.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_cloneact.cc

c_clonejoin.o: c_clonejoin.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_clonejoin.cc

c_clonepart.o: c_clonepart.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_clonepart.cc

c_clonequit.o: c_clonequit.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_clonequit.cc

c_clonesay.o: c_clonesay.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_clonesay.cc

c_ctcp.o: c_ctcp.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_ctcp.cc

c_deop.o: c_deop.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_deop.cc

c_deopall.o: c_deopall.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_deopall.cc

c_devoice.o: c_devoice.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_devoice.cc

c_devoiceall.o: c_devoiceall.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_devoiceall.cc

c_die.o: c_die.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_die.cc

c_disable.o: c_disable.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_disable.cc

c_enable.o: c_enable.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_enable.cc

c_gline.o: c_gline.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_gline.cc

c_help.o: c_help.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_help.cc

c_invite.o: c_invite.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_invite.cc

c_inviteme.o: c_inviteme.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_inviteme.cc

c_jupe.o: c_jupe.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_jupe.cc

c_kick.o: c_kick.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_kick.cc

c_login.o: c_login.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_login.cc

c_logout.o: c_logout.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_logout.cc

c_map.o: c_map.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_map.cc

c_mode.o: c_mode.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_mode.cc

c_moduser.o: c_moduser.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_moduser.cc

c_op.o: c_op.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_op.cc

c_opall.o: c_opall.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_opall.cc

c_operinfo.o: c_operinfo.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_operinfo.cc

c_opermsg.o: c_opermsg.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_opermsg.cc

c_quote.o: c_quote.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_quote.cc

c_refreshg.o: c_refreshg.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_refreshg.cc

c_remgline.o: c_remgline.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_remgline.cc

c_remuser.o: c_remuser.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_remuser.cc

c_reop.o: c_reop.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_reop.cc

c_restart.o: c_restart.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_restart.cc

c_scan.o: c_scan.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_scan.cc

c_servinfo.o: c_servinfo.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_servinfo.cc

c_set.o: c_set.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_set.cc

c_settime.o: c_settime.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_settime.cc

c_show.o: c_show.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_show.cc

c_showcommands.o: c_showcommands.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_showcommands.cc

c_translate.o: c_translate.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_translate.cc

c_unban.o: c_unban.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_unban.cc

c_unjupe.o: c_unjupe.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_unjupe.cc

c_uptime.o: c_uptime.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_uptime.cc

c_voice.o: c_voice.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_voice.cc

c_voiceall.o: c_voiceall.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_voiceall.cc

c_whois.o: c_whois.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_whois.cc

c_whoison.o: c_whoison.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_whoison.cc

c_write.o: c_write.cc
	${CPP} ${CPPFLAGS} -o $@ -c c_write.cc

chanclient.o: chanclient.cc
	${CPP} ${CPPFLAGS} -o $@ -c chanclient.cc

channel.o: channel.cc
	${CPP} ${CPPFLAGS} -o $@ -c channel.cc

client.o: client.cc
	${CPP} ${CPPFLAGS} -o $@ -c client.cc

cmdmap.o: cmdmap.cc
	${CPP} ${CPPFLAGS} -o $@ -c cmdmap.cc

config.o: config.cc
	${CPP} ${CPPFLAGS} -o $@ -c config.cc

events.o: events.cc
	${CPP} ${CPPFLAGS} -o $@ -c events.cc

gline.o: gline.cc
	${CPP} ${CPPFLAGS} -o $@ -c gline.cc

jupe.o: jupe.cc
	${CPP} ${CPPFLAGS} -o $@ -c jupe.cc

m_account.o: m_account.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_account.cc

m_admin.o: m_admin.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_admin.cc

m_away.o: m_away.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_away.cc

m_burst.o: m_burst.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_burst.cc

m_clearmode.o: m_clearmode.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_clearmode.cc

m_create.o: m_create.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_create.cc

m_destruct.o: m_destruct.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_destruct.cc

m_desynch.o: m_desynch.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_desynch.cc

m_eob.o: m_eob.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_eob.cc

m_eoback.o: m_eoback.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_eoback.cc

m_error.o: m_error.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_error.cc

m_gline.o: m_gline.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_gline.cc

m_invite.o: m_invite.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_invite.cc

m_join.o: m_join.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_join.cc

m_jupe.o: m_jupe.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_jupe.cc

m_kick.o: m_kick.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_kick.cc

m_kill.o: m_kill.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_kill.cc

m_leave.o: m_leave.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_leave.cc

m_mode.o: m_mode.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_mode.cc

m_nick.o: m_nick.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_nick.cc

m_pass.o: m_pass.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_pass.cc

m_ping.o: m_ping.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_ping.cc

m_privmsg.o: m_privmsg.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_privmsg.cc

m_quit.o: m_quit.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_quit.cc

m_server.o: m_server.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_server.cc

m_silence.o: m_silence.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_silence.cc

m_squit.o: m_squit.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_squit.cc

m_stats.o: m_stats.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_stats.cc

m_topic.o: m_topic.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_topic.cc

m_version.o: m_version.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_version.cc

m_wallops.o: m_wallops.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_wallops.cc

m_wallusers.o: m_wallusers.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_wallusers.cc

m_whois.o: m_whois.cc
	${CPP} ${CPPFLAGS} -o $@ -c m_whois.cc

main.o: main.cc
	${CPP} ${CPPFLAGS} -o $@ -c main.cc

match.o: match.cc
	${CPP} ${CPPFLAGS} -o $@ -c match.cc

msgmap.o: msgmap.cc
	${CPP} ${CPPFLAGS} -o $@ -c msgmap.cc

msgparse.o: msgparse.cc
	${CPP} ${CPPFLAGS} -o $@ -c msgparse.cc

network.o: network.cc
	${CPP} ${CPPFLAGS} -o $@ -c network.cc

server.o: server.cc
	${CPP} ${CPPFLAGS} -o $@ -c server.cc

signals.o: signals.cc
	${CPP} ${CPPFLAGS} -o $@ -c signals.cc

socket.o: socket.cc
	${CPP} ${CPPFLAGS} -o $@ -c socket.cc

stealth.o: stealth.cc
	${CPP} ${CPPFLAGS} -o $@ -c stealth.cc

timer.o: timer.cc
	${CPP} ${CPPFLAGS} -o $@ -c timer.cc

token.o: token.cc
	${CPP} ${CPPFLAGS} -o $@ -c token.cc

