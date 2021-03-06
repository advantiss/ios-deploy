"# AUTO-GENERATED - DO NOT MODIFY\n"
"import lldb\n"
"import os\n"
"import sys\n"
"import shlex\n"
"\n"
"def connect_command(debugger, command, result, internal_dict):\n"
"    # These two are passed in by the script which loads us\n"
"    connect_url = internal_dict['fruitstrap_connect_url']\n"
"    error = lldb.SBError()\n"
"\n"
"    process = lldb.target.ConnectRemote(lldb.target.GetDebugger().GetListener(), connect_url, None, error)\n"
"\n"
"    # Wait for connection to succeed\n"
"    listener = lldb.target.GetDebugger().GetListener()\n"
"    listener.StartListeningForEvents(process.GetBroadcaster(), lldb.SBProcess.eBroadcastBitStateChanged)\n"
"    events = []\n"
"    state = (process.GetState() or lldb.eStateInvalid)\n"
"    while state != lldb.eStateConnected:\n"
"        event = lldb.SBEvent()\n"
"        if listener.WaitForEvent(1, event):\n"
"            state = process.GetStateFromEvent(event)\n"
"            events.append(event)\n"
"        else:\n"
"            state = lldb.eStateInvalid\n"
"\n"
"    # Add events back to queue, otherwise lldb freezes\n"
"    for event in events:\n"
"        listener.AddEvent(event)\n"
"\n"
"def run_command(debugger, command, result, internal_dict):\n"
"    device_app = internal_dict['fruitstrap_device_app']\n"
"    args = command.split('--',1)\n"
"    error = lldb.SBError()\n"
"    lldb.target.modules[0].SetPlatformFileSpec(lldb.SBFileSpec(device_app))\n"
"    lldb.target.Launch(lldb.SBLaunchInfo(shlex.split(args[1] and args[1] or '{args}')), error)\n"
"    lockedstr = ': Locked'\n"
"    if lockedstr in str(error):\n"
"       print('\\nDevice Locked\\n')\n"
"       os._exit(254)\n"
"    else:\n"
"       print(str(error))\n"
"\n"
"def safequit_command(debugger, command, result, internal_dict):\n"
"    process = lldb.target.process\n"
"    listener = debugger.GetListener()\n"
"    listener.StartListeningForEvents(process.GetBroadcaster(), lldb.SBProcess.eBroadcastBitStateChanged | lldb.SBProcess.eBroadcastBitSTDOUT | lldb.SBProcess.eBroadcastBitSTDERR)\n"
"    event = lldb.SBEvent()\n"
"    while True:\n"
"        if listener.WaitForEvent(1, event) and lldb.SBProcess.EventIsProcessEvent(event):\n"
"            state = lldb.SBProcess.GetStateFromEvent(event)\n"
"        else:\n"
"            state = process.GetState()\n"
"\n"
"        if state == lldb.eStateRunning:\n"
"            process.Detach()\n"
"            os._exit(0)\n"
"        elif state > lldb.eStateRunning:\n"
"            os._exit(state)\n"
"\n"
"def autoexit_command(debugger, command, result, internal_dict):\n"
"    process = lldb.target.process\n"
"    listener = debugger.GetListener()\n"
"    listener.StartListeningForEvents(process.GetBroadcaster(), lldb.SBProcess.eBroadcastBitStateChanged | lldb.SBProcess.eBroadcastBitSTDOUT | lldb.SBProcess.eBroadcastBitSTDERR)\n"
"    event = lldb.SBEvent()\n"
"    while True:\n"
"        if listener.WaitForEvent(1, event) and lldb.SBProcess.EventIsProcessEvent(event):\n"
"            state = lldb.SBProcess.GetStateFromEvent(event)\n"
"        else:\n"
"            state = process.GetState()\n"
"\n"
"        if state == lldb.eStateExited:\n"
"            os._exit(process.GetExitStatus())\n"
"        elif state == lldb.eStateStopped:\n"
"            debugger.HandleCommand('bt')\n"
"            os._exit({exitcode_app_crash})\n"
"\n"
"        stdout = process.GetSTDOUT(1024)\n"
"        while stdout:\n"
"            sys.stdout.write(stdout)\n"
"            stdout = process.GetSTDOUT(1024)\n"
"\n"
"        stderr = process.GetSTDERR(1024)\n"
"        while stderr:\n"
"            sys.stdout.write(stderr)\n"
"            stderr = process.GetSTDERR(1024)\n"
