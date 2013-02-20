using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CPM_Attempt
{
   class _8080
    {
       /*-------------------------------------------------------------
        * 8080 assembly intepreter, does not emulate clock cycles
        * or anything else cycle based. (C) David Wotherspoon 2012
        * 
        * 
        * 
        * ------------------------------------------------------------*/
        bool stop = false;
        byte[] memory = new byte[65536];
        byte[] IO = new byte[256];
        byte regA;
        byte regB;
        byte regC;
        byte regD;
        byte regE;
        byte regH;
        byte regL;
        byte Flag;
        UInt16 PC;
        UInt16 SP;
        byte byte_count = 0; //used for multi-byte instructions
        public int currOpp = 0; //Stores current operation for multibyte operations
        byte temp; //stores second byte in 3 byte instructions

        public _8080(byte[] mem)
        {
        Console.WriteLine("Initalising 8080");
        PC = 0;
        Flag = 2;
        SP = UInt16.MaxValue;
        mem.CopyTo(memory, 0);
        }

        public void start()
        {
            while (!stop)
            {
                Console.WriteLine("PC: " + PC);
                Console.WriteLine("MEM: " + memory[PC]);
                Console.WriteLine("RegA: " + regA.ToString("x"));
                Console.WriteLine("SP: " + SP);
                Console.WriteLine("-------------------------------");
                execute();
                //devices
            }
            Console.WriteLine("CPU was stopped/halted.");
        }

        UInt16 byteToUInt(byte byte1, byte byte2)
        {
            UInt16 result = (UInt16)(byte2 << 8);
            result += byte1;
            return result;
        }

        byte[] UIntToByte(UInt16 val)
        {
            byte[] ret = { (byte)val, (byte)(val >> 8)};
            return ret;
        }

        void setSFlag(bool val)
        {
            if ((Flag & 128) > 0)
            {
                if (!val)
                {
                    Flag -= 128;
                }
            }
            else
            {
                if (val)
                {
                    Flag += 128;
                }
            }
        }

        void setSFlag() //sets based on Areg
        {
            if ((regA & 128) > 0)
            {
                setSFlag(true);
            }
            else
            {
                setSFlag(false);
            }
        }

        bool getSFlag()
        {
            if ((Flag & 128) > 0)
            {
                return true;
            }
            return false;
        }


        void setZFlag(bool val)
        {
            if ((Flag & 64) > 0)
            {
                if (!val)
                {
                    Flag -= 64;
                }
            }
            else
            {
                if (val)
                {
                    Flag += 64;
                }
            }
        }

        void setZFlag()
        {
            if (regA == 0)
            {
                setZFlag(true);
            }
            else
            {
                setZFlag(false);
            }
        }

        bool getZFlag()
        {
            if ((Flag & 64) > 0)
            {
                return true;
            }
            return false;
        }


        void setACFlag(bool val)
        {
            if ((Flag & 16) > 0)
            {
                if (!val)
                {
                    Flag -= 16;
                }
            }
            else
            {
                if (val)
                {
                    Flag += 16;
                }
            }
        }

        bool getACFlag()
        {
            if ((Flag & 16) > 0)
            {
                return true;
            }
            return false;
        }


        void setPFlag(bool val)
        {
            if ((Flag & 4) > 0)
            {
                if (!val)
                {
                    Flag -= 4;
                }
            }
            else
            {
                if (val)
                {
                    Flag += 4;
                }
            }
        }

        void setPFlag()
        {
            byte bcount = 0;
            for (int i = 0; i < 8; i++)
            {
                if ((regA & (2 ^ i)) > 0)
                {
                    bcount++;
                }
            }
            if (bcount % 2 == 0)
            {
                //even
                setPFlag(true);
            }
            else
            {
                setPFlag(false);
            }
        }

        bool getPFlag()
        {
            if ((Flag & 4) > 0)
            {
                return true;
            }
            return false;
        }

        void setCYFlag(bool val) //set to known value method
        {
            if ((Flag & 1) > 0)
            {
                if (!val)
                {
                    Flag -= 1;
                }
            }
            else
            {
                if (val)
                {
                    Flag += 1;
                }
            }
        }

        bool getCYFlag()
        {
            if ((Flag & 1) > 0)
            {
                return true;
            }
            return false;
        }

        void printStatus()
        {
            Console.WriteLine("Registers:");
            Console.WriteLine("A: " + regA.ToString("x"));
        }

        void execute()
        {
            if (byte_count != 0) //waiting for bytes?
            {
                switch (currOpp)
                {
                    case 1: //LXI B
                        if (byte_count == 2)
                        {
                            byte_count--;
                            temp = memory[PC];
                            PC++;
                        }
                        else
                        {
                            regB = memory[PC];
                            regC = temp;
                            byte_count--;
                            PC++;
                        }
                        break;
                    case 58: //LDA
                        if (byte_count == 2)
                        {
                            byte_count--;
                            temp = memory[PC];
                            PC++;
                        }
                        else
                        {
                            regA = memory[byteToUInt(temp, memory[PC])];
                            PC++;
                            byte_count--;
                        }
                        break;
                    case 194: //JNZ
                           if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getZFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 195: //JMP
                        {
                            if (byte_count == 2)
                            {
                                temp = memory[PC];
                                byte_count--;
                                PC++;
                            }
                            else
                            {
                                byte_count--;
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            break;
                        }
                    case 196: //CNZ
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getZFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 202: //JZ
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getZFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 204: //CZ
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getZFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 205: //CALL
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            PC++;
                            SP--;
                            memory[SP] = UIntToByte(PC)[1];
                            SP--;
                            memory[SP] = UIntToByte(PC)[0];
                            PC = byteToUInt(temp, memory[PC]);
                        }
                        break;
                    case 210: //JNC
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getCYFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 211: //OUT
                        byte_count--;
                        IO[memory[PC]] = regA;
                        PC++;
                        break;
                    case 212: //CNC
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getCYFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 218: //JC
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getCYFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 220: //CC
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getCYFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 226: //JPO
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getPFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 227: //CPO
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getPFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 230: //ANI
                        byte_count--;
                        regA = (byte)(regA & memory[PC]);
                        PC++;
                        setZFlag();
                        setSFlag();
                        setPFlag();
                        setCYFlag(false);
                        setACFlag(true);
                        break;
                    case 234: //JPE
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getPFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 236: //CPE
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getPFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 238: //XRI
                        byte_count--;
                        regA = (byte)(regA ^ memory[PC]);
                        PC++;
                        setZFlag();
                        setSFlag();
                        setPFlag();
                        setCYFlag(false);
                        setACFlag(false);
                        break;
                    case 242: //JP
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getSFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 244: //CP
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (getSFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 246: //ORI
                        byte_count--;
                        regA = (byte)(regA | memory[PC]);
                        PC++;
                        setZFlag();
                        setSFlag();
                        setPFlag();
                        setCYFlag(false);
                        setACFlag(false);
                        break;
                    case 250: //JM
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getSFlag())
                            {
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 252: //CM
                        if (byte_count == 2)
                        {
                            temp = memory[PC];
                            byte_count--;
                            PC++;
                        }
                        else
                        {
                            byte_count--;
                            if (!getSFlag())
                            {
                                PC++;
                                SP--;
                                memory[SP] = UIntToByte(PC)[1];
                                SP--;
                                memory[SP] = UIntToByte(PC)[0];
                                PC = byteToUInt(temp, memory[PC]);
                            }
                            else
                            {
                                PC++;
                            }
                        }
                        break;
                    case 254: //CPI
                        byte_count--;
                        byte var = memory[PC];
                        if (regA == var)
                        {
                            setZFlag(true);
                            setCYFlag(false);
                        }
                        else if (regA > var)
                        {
                            setZFlag(false);
                            setCYFlag(false);
                        }
                        else
                        {
                            setCYFlag(true);
                            setZFlag(false);
                        }
                        PC++;
                        break;
                }
            }
            else //fresh instruction
            {
                switch (memory[PC])
                {
                    case 0: //NOP 1
                        PC++;
                        break;
                    case 1: //LXI B 3
                        byte_count = 2; //2 more bytes to fetch
                        currOpp = 1;
                        PC++;
                        break;
                    case 2: //STAX B 1
                        memory[byteToUInt(regC, regB)] = regA;
                        PC++;
                        break;
                    case 3: //INX B 1 FLAGS
                        if (regC == 255)
                        {
                            regC = 0;
                            regB++;
                        }
                        else
                        {
                            regC++;
                        }
                        PC++;
                        break;
                    case 7: //RLC
                        byte D7 = (byte)(regA & 128); //mask out D7
                        D7 = (byte)(D7 >> 7); //shift it to D0
                        regA = (byte)(regA << 1); //Shift Accumulator 
                        regA += D7; //ADD D7 to D0
                        if (D7 > 0)
                        {
                            setCYFlag(true);
                        }
                        else
                        {
                            setCYFlag(false);
                        }
                        PC++;
                        break;
                    case 9: //DAD B
                        byte[] result = UIntToByte((UInt16)(byteToUInt(regL, regH) + byteToUInt(regC, regB)));
                        if (byteToUInt(regL, regH) + byteToUInt(regC, regB) > 65536)
                        {
                            setCYFlag(true);
                        }
                        else
                        {
                            setCYFlag(false);
                        }
                        regH = result[1];
                        regL = result[0];
                        PC++;
                        break;
                    case 10: //LDAX B Indirect B & C
                        break;
                    case 11: //DCX B
                        UInt16 val = byteToUInt(regC, regB);
                        val--;
                        byte[] resultD = UIntToByte(val);
                        regB = resultD[1];
                        regC = resultD[0];
                        PC++;
                        break;
                    case 15: //RRC
                        byte D0 = (byte)(regA & 1);
                        D0 = (byte)(D0 << 7);
                        regA = (byte)(regA >> 1);
                        regA += D0;
                        PC++;
                        if (D0 > 0)
                        {
                            setCYFlag(true);
                        }
                        else
                        {
                            setCYFlag(false);
                        }
                        break;
                    case 17: //LXI D
                        break;
                    case 18: //STAX D
                        break;
                    case 19: //INX D
                        UInt16 valD = byteToUInt(regE, regD);
                        valD++;
                        byte[] resultI = UIntToByte(valD);
                        regD = resultI[1];
                        regE = resultI[0];
                        PC++;
                        break;
                    case 23: //RAL
                        byte D7C = (byte)(regA & 128); //mask out D7
                        regA = (byte)(regA << 1); //Shift Accumulator 
                        regA += (byte)(Flag & 1); //move carry flag in
                        if (D7C > 0)
                        {
                            setCYFlag(true);
                        }
                        else
                        {
                            setCYFlag(false);
                        }
                        PC++;
                        break;
                    case 25: //DAD D
                        byte[] resultE = UIntToByte((UInt16)(byteToUInt(regL, regH) + byteToUInt(regE, regD)));
                        if (byteToUInt(regL, regH) + byteToUInt(regE, regD) > 65536)
                        {
                            setCYFlag(true);
                        }
                        else
                        {
                            setCYFlag(false);
                        }
                        regH = resultE[1];
                        regL = resultE[0];
                        PC++;
                        break;
                    case 26: //LDAX D
                        break;
                    case 27: //DCX D
                        UInt16 valF = byteToUInt(regE, regD);
                        valF--;
                        byte[] resultF = UIntToByte(valF);
                        regD = resultF[1];
                        regE = resultF[0];
                        PC++;
                        break;
                    case 31: //RAR
                        byte D0C = (byte)(regA & 1); //mask out D0
                        regA = (byte)(regA >> 1); //Shift Accumulator 
                        regA += (byte)((Flag & 1) << 7); //move carry flag in
                        if (D0C > 0)
                        {
                            setCYFlag(true);
                        }
                        else
                        {
                            setCYFlag(false);
                        }
                        PC++;
                        break;
                    case 33: //LXI H
                        break;
                    case 34: //SHLD
                        break;
                    case 35: //INX H
                        break;
                    case 39: //DAA

                        break;
                    case 41: //DAD H
                        break;
                    case 42: //LHLD
                        break;
                    case 43: //DCX H
                        break;
                    case 47: //CMA
                        regA = (byte)(~regA);
                        PC++;
                        break;
                    case 49: //LXI SP
                        break;
                    case 50: //STA
                        break;
                    case 51: //INX SP
                        break;
                    case 52: //INRm
                        break;
                    case 53: //DCRm
                        break;
                    case 54: //MVI, m
                        break;
                    case 55: //STC 1
                        setCYFlag(true);
                        PC++;
                        break;
                    case 57: //DAD SP
                        break;
                    case 58: //LDA DIRECT
                        currOpp = 58;
                        byte_count = 2;
                        PC++;
                        break;
                    case 59: //DCX SP
                        break;
                    case 66: //CMC 1
                        setCYFlag(!getCYFlag());
                        PC++;
                        break;
                    case 118: //HALT 1
                        PC++;
                        stop = true;
                        break;
                    case 134: //ADDm
                        break;
                    case 142: //ADCm
                        break;
                    case 150: //SUBm
                        break;
                    case 158: //SBBm
                        break;
                    case 166: //ANAm
                        regA = (byte)(memory[byteToUInt(regL, regH)] & regA);
                        PC++;
                        setZFlag();
                        setSFlag();
                        setPFlag();
                        setCYFlag(false);
                        setACFlag(true);
                        break;
                    case 174: //XRAm
                        regA = (byte)(regA ^ memory[byteToUInt(regL, regH)]);
                        PC++;
                        setZFlag();
                        setSFlag();
                        setPFlag();
                        setCYFlag(false);
                        setACFlag(false);
                        break;
                    case 182: //ORAm
                        regA = (byte)(regA | memory[byteToUInt(regL, regH)]);
                        PC++;
                        setZFlag();
                        setSFlag();
                        setPFlag();
                        setCYFlag(false);
                        setACFlag(false);
                        break;
                    case 190: //CMPm
                        byte var = memory[byteToUInt(regL, regH)];
                        if (regA == var)
                        {
                            setZFlag(true);
                            setCYFlag(false);
                        }
                        else if (regA > var)
                        {
                            setZFlag(false);
                            setCYFlag(false);
                        }
                        else
                        {
                            setCYFlag(true);
                            setZFlag(false);
                        }
                        PC++;
                        break;
                    case 192: //RNZ
                        if (!getZFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 193: //POP B
                        break;
                    case 194: //JNZ
                        byte_count = 2;
                        currOpp = 194;
                        PC++;
                        break;
                    case 195: //JMP
                        byte_count = 2;
                        PC++;
                        currOpp = 195;
                        break;
                    case 196: //CNZ
                        byte_count = 2;
                        PC++;
                        currOpp = 196;
                        break;
                    case 197: //PUSH B
                        break;
                    case 198: //ADI
                        break;
                    case 200: //RZ
                        if (getZFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 201: //RET
                        temp = memory[SP];
                        SP++;
                        PC = byteToUInt(temp, memory[SP]);
                        SP++;
                        break;
                    case 202: //JZ 
                        byte_count = 2;
                        currOpp = 202;
                        PC++;
                        break;
                    case 204: //CZ
                        byte_count = 2;
                        currOpp = 204;
                        PC++;
                        break;
                    case 205: //CALL
                        byte_count = 2;
                        PC++;
                        currOpp = 205;
                        break;
                    case 206: //ACI
                        break;
                    case 208: //RNC
                        if (!getCYFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 209: //POP D
                        break;
                    case 210: //JNC
                        byte_count = 2;
                        PC++;
                        currOpp = 210;
                        break;
                    case 211: //OUT
                        byte_count = 1;
                        PC++;
                        currOpp = 211;
                        break;
                    case 212: //CNC
                        byte_count = 2;
                        PC++;
                        currOpp = 212;
                        break;
                    case 213: //PUSH D
                        break;
                    case 214: //SUI
                        break;
                    case 216: //RC
                        if (getCYFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 218: //JC
                        byte_count = 2;
                        PC++;
                        currOpp = 218;
                        break;
                    case 219: //IN
                        break;
                    case 220: //CC
                        byte_count = 2;
                        PC++;
                        currOpp = 220;
                        break;
                    case 222: //SBI
                        break;
                    case 224: //RPO
                        if (!getPFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 225: //POP H
                        break;
                    case 226: //JPO
                        byte_count = 2;
                        currOpp = 226;
                        PC++;
                        break;
                    case 227: //XTHL
                        break;
                    case 228: //CPO
                        byte_count = 2;
                        PC++;
                        currOpp = 228;
                        break;
                    case 229: //PUSH H
                        break;
                    case 230: //ANI
                        byte_count = 1;
                        PC++;
                        currOpp = 230;
                        break;
                    case 232: //RPE
                        if (getPFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 233: //PCHL
                        PC = byteToUInt(regL, regH);
                        break;
                    case 234: //JPE
                        byte_count = 2;
                        currOpp = 234;
                        PC++;
                        break;
                    case 235: //XCHG
                        break;
                    case 236: //CPE
                        byte_count = 2;
                        currOpp = 236;
                        PC++;
                        break;
                    case 238: //XRI
                        byte_count = 1;
                        PC++;
                        currOpp = 238;
                        break;
                    case 240: //RP
                        if (!getSFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 241: //POP PSW
                        break;
                    case 242: //JP
                        byte_count = 2;
                        PC++;
                        currOpp = 242;
                        break;
                    case 243: //DI
                        PC++;
                        //Unused for now, Disable Interrupts
                        break;
                    case 244: //CP
                        byte_count = 2;
                        PC++;
                        currOpp = 244;
                        break;
                    case 245: //PUSH PSW
                        break;
                    case 246: //ORI
                        byte_count = 1;
                        currOpp = 246;
                        break;
                    case 248: //RM
                        if (getSFlag())
                        {
                            temp = memory[SP];
                            SP++;
                            PC = byteToUInt(temp, memory[SP]);
                            SP++;
                        }
                        else
                        {
                            PC++;
                        }
                        break;
                    case 249: //SPHL
                        break;
                    case 250: //JM
                        byte_count = 2;
                        currOpp = 250;
                        PC++;
                        break;
                    case 251: //EI 1
                        PC++;
                        //Unused for now, Enable Interrupts
                        break;
                    case 252: //CM
                        byte_count = 2;
                        PC++;
                        currOpp = 252;
                        break;
                    case 254: //CPI
                        byte_count = 1;
                        PC++;
                        currOpp = 254;
                        break;
                }
            }
        }
    }
}