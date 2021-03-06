# Compact packet log format

The cplf is a stream format, i.e. packets are encoded in a bytestream with independent chunks

Byte oder is little endian

## Format

    ICMP                = 0b00000001
    TCP                 = 0b00000010
    UDP                 = 0b00000011
    
    ICMP_PING_REQUEST   = 0b00000100
    ICMP_OTHER          = 0b00001000
    
    TCP_SYN             = 0b00000100
    TCP_ACK             = 0b00001000
    TCP_PSH             = 0b00010000
    TCP_RST             = 0b00100000
    TCP_FIN             = 0b01000000
    
    [Stream]    := n*[Chunk]
    [Chunk]     := [type: uint8_t][time: uint32_t][source: uint32_t]( type & TCP || type == UDP ? [dport: uint16_t])
    
    type        : (ICMP && (ICMP_PING_REQUEST || ICMP_OTHER) || TCP && (TCP_ACK || TCP_SYN || TCP_PSH || TCP_RST || TCP_FIN) || UDP)
    source      : IPv4


