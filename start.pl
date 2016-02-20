#!/usr/bin/perl

use 5.010;
use Mojo::Webqq;
use Mojo::Util qw(md5_sum);

my $qq = undef;

sub create_account {
	open(AFILE, ">./account.txt");
	print "请输入QQ号： ";
	chomp($qq = <STDIN>);
	print AFILE $qq . "\n#请不要修改此文件，登录失败时，可删除此文件重新登录进行键入账号";
	close(AFILE);
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
	say "发现历史账号，正在读取账号配置...";
	open (AFILE, "<./account.txt");
	chomp($qq = <AFILE>);
	chomp($pwd = <AFILE>);
	close(AFILE);
	if($qq && $pwd) {
		say "读取成功";
	} else {
		say "账号文件为空，请重新登录";
		&create_account;
	}
} else {
	&create_account;
}

#say "debug  ".$qq."\n".$pwd;
my $client = Mojo::Webqq->new(
	ua_debug	=> 0,
	qq		=> $qq,
	login_type	=> "qrlogin",
);
$client->login();
$client->load("ShowMsg");

#$client->load("PostQRcode",data=>{
#	smtp	=> '',
#	port	=> '',
#	from	=> '',
#	to	=> '',
#	user	=> '',
#	pass	=> '',
#});

$client->on(receive_message=>sub{
	my ($client,$msg) = @_;
	if($msg->type eq 'message') {
		$client->spawn(
			cmd		=> sub { system('./core/start.sh', $msg->type, $msg->sender->qq, $msg->content)},
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

