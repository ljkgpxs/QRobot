#!/usr/bin/perl

use 5.010;
use Mojo::Webqq;
use Mojo::Util qw(md5_sum);

my @required_mojo_webqq_ver = ( '1', '7', '9' );

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

#say "debug  ".$qq."\n".$pwd;
my $client = Mojo::Webqq->new(
	ua_debug	=> 0,
#	qq		=> $qq,
	login_type	=> "qrlogin",
);

# 判断Mojo-Webqq版本是否达到要求
my @version = split(/\./, $client->version);
my $i = 0;
while($version[$i] - $required_mojo_webqq_ver[$i] == 0) {
	$i++;
	if($i == 3) {
		last;
	}
}
if($i != 3 && $version[$i] - $required_mojo_webqq_ver[$i] < 0) {
	say "Your Mojo-Webqq version v$version[0].$version[1].$version[2] < v$required_mojo_webqq_ver[0].$required_mojo_webqq_ver[1].$required_mojo_webqq_ver[2], Please update your Mojo-Webqq and retry.";
	exit(1);
}

$client->on("input_qrcode"=>sub{
		my ($client, $qrpath) = @_;
#		system('./tools/viewqr', $qrpath),
		$client->spawn(
			is_blocking 	=> 1,
			cmd		=> sub{ system('./tools/viewqr', '-png', $qrpath) },
			exec_timeout	=> 2,
			stdout_cb	=> sub { 
				my($pid, $chunk) = @_;
				print $chunk;
			},
			exit_cb		=> sub{},
		);
});

# 开始登录
$client->login();
$client->load("ShowMsg");

$client->on(receive_message=>sub{
	my ($client,$msg) = @_;
	if($msg->type eq 'message') {
		$client->spawn(
			cmd		=> sub { system('./core/start.sh', $msg->type, $msg->sender->qq, $msg->content) },
			exec_timeout	=> 5,
			exit_cb		=> sub{
				my @reply_content = &passmsg($msg->sender->qq);
				if(@reply_content) {
					my $reply_text;
					foreach(@reply_content) {
						$reply_text .= $_;
					}
					$msg->reply($reply_text);
				} else {
					say "System message: No Reply";
				}
			},
		);
	}
#	$client->reply_message($msg,$msg->{content});
});

$client->run;
system("rm -f /tmp/*.reply >/dev/null 2>/dev/null");

