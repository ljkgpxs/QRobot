#!/usr/bin/perl

use 5.010;
use Mojo::Webqq;
use Mojo::Util qw(md5_sum);

my $qq = undef;
my $pwd = undef;

sub create_account {
	open(AFILE, ">./account.txt");
	print "请输入QQ号： ";
	chomp($qq = <STDIN>);
	print "请输入密码： ";
	chomp($pwd = <STDIN>);
	$pwd = md5_sum($pwd);
	print AFILE $qq . "\n" . $pwd;
}

sub read_from_file
{
	my $to_file = '/tmp/' . $_[0] . '.reply';
	if(open(FILE, "<$to_file")) {
		my @content = <FILE>;
		close(FILE);
		@content;
	}
	else {
		return 0;
	}
}

sub passmsg
{
	&read_from_file($_[0]);
}

if(-e "./account.txt") {
	open (AFILE, "<./account.txt");
	chomp($qq = <AFILE>);
	chomp($pwd = <AFILE>);
	close(AFILE);
	if($qq && $pwd) {
	} else {
		&create_account;
	}
} else {
	&create_account;
}

#say "debug  ".$qq."\n".$pwd;
my $client = Mojo::Webqq->new(ua_debug=>0);
$client->login(qq => $qq, pwd => $pwd);
$client->load("ShowMsg");
$client->on(receive_message=>sub{
	my ($client,$msg) = @_;
	if($msg->type eq 'message') {
		system("./core/start.sh", $msg->type, $msg->sender->qq, $msg->content);
		my @reply_content = &passmsg($msg->sender->qq);
#		say "Debug   " . $msg->sender->qq;
		if(@reply_content) {
			my $reply_text;
			foreach (@reply_content) {
				$reply_text .= $_;
			}
			$client->reply_message($msg, $reply_text);
		} else {
			say "";
			say "System message:No Reply";
		}
	}
	$client->reply_message($msg,$msg->{content});
});

$client->run;
system("rm /tmp/*.reply");

