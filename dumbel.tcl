set ns [new Simulator]
set tr [open samp.tr w]
$ns trace-all $tr
set nm [open fir.nam w]
$ns namtrace-all $nm

# Creating nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n0 $n2 1Mb 10ms DropTail
$ns duplex-link $n0 $n3 1Mb 10ms DropTail
$ns duplex-link $n0 $n4 1Mb 10ms DropTail
$ns duplex-link $n0 $n5 1Mb 10ms DropTail

set tcp1 [new Agent/TCP]
$ns attach-agent $n1 $tcp1
set sink0 [new Agent/TCPSink]
$ns attach-agent $n0 $sink0
set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1



set tcp2 [new Agent/TCP]
$ns attach-agent $n2 $tcp2
set sink5 [new Agent/TCPSink]
$ns attach-agent $n5 $sink5
set ftp2 [new Application/FTP]
$ftp2 attach-agent $tcp2

set udp4 [new Agent/UDP]
$ns attach-agent $n4 $udp4
set cbr4 [new Application/Traffic/CBR]
$cbr4 attach-agent $udp4
set null3 [new Agent/Null]
$ns attach-agent $n3 $null3

proc finish {} {
    puts "Simulation complete"
    exit 0
}

$ns connect $tcp1 $sink0
$ns connect $tcp2 $sink5
$ns connect $udp4 $null3
$ns at 0.1 "$ftp1 start"
$ns at 1.0 "$ftp2 start"
$ns at 1.5 "$cbr4  start"
$ns at 2.5 "$ftp1 stop"
$ns at 2.8 "$cbr4 stop"
$ns at 3.1 "$ftp2 stop"
$ns at 4.0 "finish"

$ns run

