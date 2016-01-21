// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__ENGINE__MSG__HPP__
#define __VHFSIM__ENGINE__MSG__HPP__

#include <cstdint>
#include <string>

namespace vhfsim
{
namespace engine
{
enum MSG_TYPE {
	MSG_VHF = 0x0000, // normal message, default
	MSG_PULSE = 0x0001 // used to trigger 'ether'
};

enum DSC_TYPE {
	DSC_INVALID_TYPE = 0x0000,
	DSC_ROUTINE = 0x0001,
	DSC_SAFETY = 0x0002,
	DSC_URGENCY = 0x0003,
	DSC_ALERT = 0x0004,
	DSC_ACK = 0x1000
};

enum DSC_RECV {
	DSC_INVALID_RECV = 0x0000,
	DSC_ALL = 0x0001,
	DSC_GROUP = 0x0002,
	DSC_INDIVIDUAL = 0x0003
};

enum DSC_DESIGNATION {
	DSC_DES_UNDEFINED = 0,
	DSC_DES_ABANDONING = 1,
	DSC_DES_PIRACY = 2,
	DSC_DES_MOB = 3,
	DSC_DES_FIRE = 4,
	DSC_DES_FLOODING = 5,
	DSC_DES_COLLISION = 6,
	DSC_DES_GROUNDING = 7,
	DSC_DES_LISTING = 8,
	DSC_DES_SINKING = 9,
	DSC_DES_ADRIFT = 10
};

struct msg_t {
	msg_t();

	uint16_t type; // type of the message

	uint16_t valid_lat; // 0=invalid, 1=valid
	uint16_t valid_lon; // 0=invalid, 1=valid
	uint16_t valid_time; // 0=invalid, 1=valid

	char lat[9]; // string representation of latitude "xx-xx.xy"
	char lon[10]; // string representation of longitude "xxx-xx.xy"
	char time[20]; // string representation of time in UTC "yyyy-mm-dd hh:mm:ss"

	char mmsi[10]; // the ships MMSI
	char group[10]; // the group MMSI, empty if not configured

	uint16_t channel; // the channel on which the call is done
	uint16_t work_channel; // the working channel
	uint16_t power; // the output power at which the transmission takes place

	struct dsc_t {
		uint16_t type; // DSC call type (routine, safety, urgency, alert)
		uint16_t recv; // receiver to whom the call goes (all, group, individual)
		uint16_t designation; // DSC alert designation
		char mmsi[10]; // the receiver (only if individual or group) MMSI if needed
	} dsc;

} __attribute__((packed));

void msg_dump(const msg_t &);
msg_t hton(const msg_t &);
msg_t ntoh(const msg_t &);

void msg_set_latitude(msg_t &, const std::string &);
void msg_set_longitude(msg_t &, const std::string &);
}
}

#endif
