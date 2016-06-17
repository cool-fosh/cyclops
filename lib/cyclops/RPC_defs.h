/** @file RPC_defs.h
    @brief Defines a few constants of the RPC Format Specification.
           This file is part of CL.
    
    @page rpc-desc RPC Format Description

    @tableofcontents

    @section rpc-overview Overview
    There are two kinds of Serial packets, single byte and multi byte.
    The size of a packet is determined by MSB of header and the ``command`` field.

    @section S-header-desc Single Byte Packets
    Single byte Headers are distinguished from other headers by the MSB bit. For
    Single byte headers, this bit is always set
    
    Field       | Bits  | Description
    ----------- | ----- | -----------
    Reserved    | [7]   | Always ``1``.
    ``channel`` | [6:3] | Determines if "command" is applied on the Channel @par 1: Command is appplied on Channel
    ``command`` | [2:0] | The command field
    
    @subsection S-cmd-desc Command descriptions
    
    command[3:0] | Name     | Effect
    ------------ | -------  | -------------
    ``0000``     | start    | Launch mainloop
    ``0001``     | stop     | Pause mainloop
    ``0010``     | reset    | Reset all sources. @attention The system is *not* reset to _initial configuration_!
    ``0011``     | swap     | Swap the Cyclops instances of the 2 high ``channel`` bits.
    ``1111``     | identity | Send device description
    
    @section M-header-desc Multi Byte Packets
    Packet is formed by concatenating the header with argument bytes.

    Field       | Bits  | Description
    ----------- | ----- | -----------
    Reserved    | [7]   | Always ``0``.
    ``channel`` | [6:5] | Command is appplied on Channel ``channel[1:0]``
    ``command`` | [4:0] | The command field

    @subsection M-cmd-desc Command descriptions

    command[4:0]  | Name               | Size(Bytes) | Effect
    ------------- | ------------------ | ----------- | --------
    ``00000``     | change_source_l    | 2           | Changes Source instance to the one reffered by @ref src-id-sec. Mode is set to ``LOOPBACK``.
    ``00001``     | change_source_o    | 2           | Changes Source instance to the one reffered by @ref src-id-sec. Mode is set to ``ONE_SHOT``.
    ``00010``     | change_source_n    | 3           | Changes Source instance to the one reffered by @ref src-id-sec. Mode is set to ``N_SHOT``. \f$N\f$ is set to @ref shot_cycle "shot_cycle"
    ``00011``     | change_time_period | 5           | Set time period of updates @attention Works only if Source::holdTime is a constant!
    ``00101``     | time_factor        | 5           | Scale Source::holdTime values by this factor. \f$\in (0, \infty)\f$.
    ``00111``     | voltage_offset     | 3           | Add this DC offset level to Source::getVoltage values. \f$\in (0, \infty)\f$.
    ``01000``     | square_on_time     | 5           | Set squareSource::onTime.
    ``01001``     | square_off_time    | 5           | Set squareSource::offTime.

    @note       Unlike providing a scaling factor for time, we don't provide a
                voltage scaling factor. Voltage scaling is accomplished by the
                GAIN knob on Cyclops Front Panel.

    @subsubsection src-id-sec src_id
    Each Source has a unique ID which is internally used by Task. The OE plugin
    can just use the number as a reference. @sa Source::src_id
    
    @subsection M-arg-byte-desc Argument Bytes

    | Command Name       | Argument[0]   | Argument[1]      |
    | ------------------ | ------------- | ---------------- |
    | change_source_l    | uint8  src_id |                  |
    | change_source_o    | uint8  src_id |                  |
    | change_source_n    | uint8  src_id | @anchor shot_cycle uint8 shot_cycle |
    | change_time_period | uint32 val    |                  |
    | time_factor        | float  val    |                  |
    | voltage_offset     | uint16 val    |                  |
    | square_on_time     | uint32 val    |                  |
    | square_off_time    | uint32 val    |                  |

    @sa Task

    @author Ananya Bahadur
*/

#ifndef CL_RPC_DEFS_h
#define CL_RPC_DEFS_h

#define RPC_HEADER_SZ 1
#define RPC_CHANNEL_RSHIFT 6
#define RPC_CHANNEL_MASK   0x02
#define RPC_COMMAND_RSHIFT 0
#define RPC_COMMAND_MASK   0x3f

#define EXTRACT_CHANNEL(header_byte) ((header_byte >> RPC_CHANNEL_RSHIFT) & RPC_CHANNEL_MASK)
#define EXTRACT_COMMAND(header_byte) ((header_byte >> RPC_COMMAND_RSHIFT) & RPC_COMMAND_MASK)
#endif