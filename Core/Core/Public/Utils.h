#pragma once
#include "pch.h"

#include <unordered_map>

#include "Configuration.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectMacros.h"

class UObject;

class Utils {
public:
	static void InitConsole(FCoreConfig& Config);

	static uintptr_t ResolveRipRelative(uintptr_t instr, int dispOffset, int instrLen);

	static void DumpGObjects(bool bLog = false);
	static void DumpClassProperties(const char* ClassName);
	static void DumpEnumProperties(const char* EnumName);
	static uint8 GetEnumValueFromName(const char* EnumName, const char* EnumMemberName);

	static void Hook();

	static FString GetDefaultMapName(FCoreConfig& Config);

	static FString BuildTravelURL(FString MapPath, std::vector<std::pair<std::string, std::string>> Options = {});

	static void SetLogVerbosity();

	static void RemoveLocalPlayer();

	static bool SetupDedicatedServer(FCoreConfig& Config);

	static bool LoadWorld(FCoreConfig& Config);

    static uintptr_t GetCallDestination(uintptr_t callAddr);

    static double NowSeconds();

	static UObject* GetObjectFromString(const std::string& InString, EClassCastFlags Flags = EClassCastFlags::RF_NoFlags);
	static UObject* GetObjectFromString(const std::string& InString, UClass* Class = nullptr);

    static wchar_t* StringToWChar(const std::string& input);

	static std::string StringToLower(const std::string& str);
};

class FCommandParser
{
private:
    std::vector<std::string> Args;
    std::unordered_map<std::string, std::string> NamedArgs;
    std::string FullCommand;

public:
    FCommandParser(const std::string& Command)
        : FullCommand(Command)
    {
        ParseArguments(Command);
    }

    std::string GetCommand() const
    {
        return Args.empty() ? "" : Args[0];
    }

    int32 GetArgCount() const
    {
        return Args.size() > 0 ? (int32)(Args.size() - 1) : 0;
    }

    bool HasNamedArg(const std::string& Name) const
    {
        return NamedArgs.find(Utils::StringToLower(Name)) != NamedArgs.end();
    }

    std::string GetNamedArg(const std::string& Name, const std::string& DefaultValue = "") const
    {
        auto It = NamedArgs.find(Utils::StringToLower(Name));
        return It != NamedArgs.end() ? It->second : DefaultValue;
    }

    std::string GetArg(int32 Index) const
    {
        int32 ActualIndex = Index + 1;
        if (ActualIndex >= 0 && ActualIndex < (int32)Args.size())
            return Args[ActualIndex];
        return "";
    }

    std::string GetArg(const std::string& Name, int32 Index, const std::string& DefaultValue = "") const
    {
        if (HasNamedArg(Name))
            return GetNamedArg(Name);

        std::string Arg = GetArg(Index);
        return Arg.empty() ? DefaultValue : Arg;
    }

    int32 GetArgInt(int32 Index, int32 DefaultValue = 0) const
    {
        return ParseInt(GetArg(Index), DefaultValue);
    }

    int32 GetArgInt(const std::string& Name, int32 Index, int32 DefaultValue = 0) const
    {
        return ParseInt(GetArg(Name, Index), DefaultValue);
    }

    float GetArgFloat(int32 Index, float DefaultValue = 0.0f) const
    {
        return ParseFloat(GetArg(Index), DefaultValue);
    }

    float GetArgFloat(const std::string& Name, int32 Index, float DefaultValue = 0.0f) const
    {
        return ParseFloat(GetArg(Name, Index), DefaultValue);
    }

    bool GetArgBool(int32 Index, bool DefaultValue = false) const
    {
        return ParseBool(GetArg(Index), DefaultValue);
    }

    bool GetArgBool(const std::string& Name, int32 Index, bool DefaultValue = false) const
    {
        return ParseBool(GetArg(Name, Index), DefaultValue);
    }

    std::string GetRemainingArgs(int32 StartIndex = 0) const
    {
        int32 ActualIndex = StartIndex + 1;
        if (ActualIndex >= (int32)Args.size())
            return "";

        std::string result;
        for (int32 i = ActualIndex; i < (int32)Args.size(); ++i)
        {
            if (!result.empty())
                result += " ";
            result += Args[i];
        }
        return result;
    }

    bool IsCommand(const std::string& CommandName) const
    {
        std::string cmd = GetCommand();
        std::string target = CommandName;

        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);

        return cmd == target;
    }

    const std::string& GetFullCommand() const
    {
        return FullCommand;
    }

private:
    static int32 ParseInt(const std::string& Value, int32 DefaultValue)
    {
        if (Value.empty())
            return DefaultValue;

        try {
            return std::stoi(Value);
        }
        catch (...) {
            return DefaultValue;
        }
    }

    static float ParseFloat(const std::string& Value, float DefaultValue)
    {
        if (Value.empty())
            return DefaultValue;

        try {
            return std::stof(Value);
        }
        catch (...) {
            return DefaultValue;
        }
    }

    static bool ParseBool(const std::string& Value, bool DefaultValue)
    {
        if (Value.empty())
            return DefaultValue;

        std::string lower = Value;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower == "true" || lower == "1" || lower == "yes" || lower == "on")
            return true;
        if (lower == "false" || lower == "0" || lower == "no" || lower == "off")
            return false;

        return DefaultValue;
    }

    void AddNamedArg(const std::string& Piece, size_t Equals)
    {
        std::string Name = (Equals == std::string::npos) ? Piece : Piece.substr(0, Equals);
        std::string Value = (Equals == std::string::npos) ? "true" : Piece.substr(Equals + 1);

        std::transform(Name.begin(), Name.end(), Name.begin(), ::tolower);
        if (!Name.empty())
            NamedArgs[Name] = Value;
    }

    void ParseArguments(const std::string& Command)
    {
        bool inQuotes = false;
        std::string currentArg;

        std::vector<std::string> Tokens;
        for (char c : Command)
        {
            if (c == '"')
            {
                inQuotes = !inQuotes;
            }
            else if (c == ' ' && !inQuotes)
            {
                if (!currentArg.empty())
                {
                    Tokens.push_back(currentArg);
                    currentArg.clear();
                }
            }
            else
            {
                currentArg += c;
            }
        }

        if (!currentArg.empty())
            Tokens.push_back(currentArg);

        for (const std::string& Token : Tokens)
        {
            size_t Cursor = 0;
            bool bFirstPiece = true;

            while (true)
            {
                size_t Next = Token.find('?', Cursor);
                std::string Piece = (Next == std::string::npos) ? Token.substr(Cursor) : Token.substr(Cursor, Next - Cursor);

                if (!Piece.empty())
                {
                    size_t Equals = Piece.find('=');

                    if (bFirstPiece && Args.empty())
                        Args.push_back(Piece);
                    else if (!bFirstPiece || Equals != std::string::npos)
                        AddNamedArg(Piece, Equals);
                    else
                        Args.push_back(Piece);
                }

                bFirstPiece = false;
                if (Next == std::string::npos)
                    break;
                Cursor = Next + 1;
            }
        }
    }
};