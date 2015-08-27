#!/usr/bin/perl

use 5.010;
use Mojo::Webqq;
use Mojo::Util qw(md5_sum);

# 在此键入QQ号和密码
my $qq = 123456789;
my $pwd = md5_sum('password');
my $client = Mojo::Webqq->new(ua_debug=>0);
$client->login(qq => $qq, pwd => $pwd);

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


$client->load("ShowMsg");
$client->on(receive_message=>sub{
    my ($client,$msg) = @_;
   
    if($msg->type eq 'message')
    {
	system("./core/start.sh", $msg->type, $msg->sender->qq, $msg->content);
    	my @reply_content = &passmsg($msg->sender->qq);
#	say "Debug   " . $msg->sender->qq;
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
#    $client->reply_message($msg,$msg->{content});
});
$client->run;
system("rm /tmp/*.reply");

