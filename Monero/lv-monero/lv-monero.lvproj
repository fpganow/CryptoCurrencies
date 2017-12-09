<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="CryptoNight" Type="Folder">
			<Item Name="CryptoNight-ParseLine.vi" Type="VI" URL="../CryptoNight/CryptoNight-ParseLine.vi"/>
			<Item Name="Test-AES-Single-Round-Host.vi" Type="VI" URL="../CryptoNight/Test-AES-Single-Round-Host.vi"/>
		</Item>
		<Item Name="Host-Implementation" Type="Folder">
			<Item Name="Add Roundkey.vi" Type="VI" URL="../CryptoNight/Host-Implementation/Add Roundkey.vi"/>
			<Item Name="AxB.vi" Type="VI" URL="../CryptoNight/Host-Implementation/AxB.vi"/>
			<Item Name="CryptoNight-SingleRound-Top-Level.vi" Type="VI" URL="../CryptoNight/Host-Implementation/CryptoNight-SingleRound-Top-Level.vi"/>
			<Item Name="Convert-U32-U8-Array.vi" Type="VI" URL="../CryptoNight/Host-Implementation/Convert-U32-U8-Array.vi"/>
			<Item Name="Create S box Number.vi" Type="VI" URL="../CryptoNight/Host-Implementation/Create S box Number.vi"/>
			<Item Name="MixColumn.vi" Type="VI" URL="../CryptoNight/Host-Implementation/MixColumn.vi"/>
			<Item Name="Shift Rows.vi" Type="VI" URL="../CryptoNight/Host-Implementation/Shift Rows.vi"/>
			<Item Name="xtime.vi" Type="VI" URL="../CryptoNight/Host-Implementation/xtime.vi"/>
		</Item>
		<Item Name="ArrayToHexString.vi" Type="VI" URL="../CryptoNight/ArrayToHexString.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="user.lib" Type="Folder">
				<Item Name="Convert EOLs (String Array)__ogtk.vi" Type="VI" URL="/&lt;userlib&gt;/_OpenG.lib/string/string.llb/Convert EOLs (String Array)__ogtk.vi"/>
				<Item Name="Convert EOLs (String)__ogtk.vi" Type="VI" URL="/&lt;userlib&gt;/_OpenG.lib/string/string.llb/Convert EOLs (String)__ogtk.vi"/>
				<Item Name="Convert EOLs__ogtk.vi" Type="VI" URL="/&lt;userlib&gt;/_OpenG.lib/string/string.llb/Convert EOLs__ogtk.vi"/>
				<Item Name="Multi-line String to Array__ogtk.vi" Type="VI" URL="/&lt;userlib&gt;/_OpenG.lib/string/string.llb/Multi-line String to Array__ogtk.vi"/>
				<Item Name="String to 1D Array__ogtk.vi" Type="VI" URL="/&lt;userlib&gt;/_OpenG.lib/string/string.llb/String to 1D Array__ogtk.vi"/>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
