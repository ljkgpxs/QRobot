#!/usr/bin/perl

use 5.014;
use Webqq::Client;
use Digest::MD5 qw(md5_hex);

# 键入QQ号和密码
my $qq = 123456789;
my $pwd = md5_hex('password');
my $client = Webqq::Client->new(debug=>0, type=>"smartqq");
$client->login( qq=> $qq, pwd => $pwd);

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
$client->on_send_message = sub{
    my ($msg,$is_success,$status) = @_;

    #使用ShowMsg插件打印发送的消息
    $client->call("ShowMsg",$msg);
};

#设置接收到消息后的回调函数
$client->on_receive_message = sub{
    #$msg = {
    #    type        => message|group_message 消息类型
    #    msg_id      => 系统生成的消息id
    #    from_uin    => 消息发送者uin，回复消息时需要用到
    #    to_uin      => 消息接受者uin，就是自己的qq
    #    content     => 消息内容，采用UTF8编码
    #    msg_time    => 消息的接收时间
    #    ttl
    #    msg_class
    #    allow_plugin
    #}
    my $msg = shift;
   
    if($msg->type eq 'message')
    {
	$client->call("ShowMsg", $msg);
	system("./core/start.sh", $msg->type, $msg->from_qq, $msg->content);
    	my @reply_content = &passmsg($msg->from_qq);
	if(@reply_content) {
		my $reply_text;
		foreach (@reply_content) {
			$reply_text .= $_;
		}
		$client->reply_message($msg, $reply_text);
	} else {
		say "System message:No Reply";
	}
    }
#    $client->call("ShowMsg",$msg);
#    $client->reply_message($msg,$msg->{content});
};
$client->run;
system("rm /tmp/*.reply");

