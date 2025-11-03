using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Wopr.Constants;
using Wopr.Entities;
using Wopr.Strategies;

namespace Wopr
{
    // Spins up clients and assigns strategies to them. 


    public class TeamDirector // : IDisposable
    {
        public delegate StrategyBase StrategyInstanceCreationDelegate();
        
        private CancellationTokenSource _cancellationTokenSource;

        private readonly string _lobbyAddress;
        private readonly string _botAuthenticationGuid;
        //private Dictionary<StrategyID, StrategyInstanceCreationDelegate> _strategiesByStrategyID;
        //private AppDomain _currentAppDomain;
        //private AssemblyLoader _assemblyLoader;

        private Dictionary<String, AllegianceInterop.ClientConnection> _connectedClientsByPlayerName = new Dictionary<String, AllegianceInterop.ClientConnection>();
        
        private Dictionary<String, StrategyBase> _currentStrategyByPlayerName { get; set; } = new Dictionary<string, StrategyBase>();

        //private Dictionary<String, AllegianceInterop.ClientConnection.OnAppMessageDelegate> _currentStrategyAppMessageDelegateByPlayerName { get; set; } = new Dictionary<string, AllegianceInterop.ClientConnection.OnAppMessageDelegate>();
        //private Dictionary<string, TeamDirectorPlayerInfo> _teamDirectorPlayerInfoByPlayerName = new Dictionary<string, TeamDirectorPlayerInfo>();

        //private List<GameInfo> _gameInfos;
        private GameInfo _gameInfo = new GameInfo();

        //[NonSerialized]
        //Task _messagePump;

        public TeamDirector(CancellationTokenSource cancellationTokenSource, string lobbyAddress)
        {
            _cancellationTokenSource = cancellationTokenSource;
            _lobbyAddress = lobbyAddress;

            //_gameInfos = new List<GameInfo>();
            //for (int i = 0; i < 6; i++)
            //{
            //    var gameInfo = new GameInfo();
            //    //gameInfo.UnexploredClustersByObjectID = new Dictionary<int, string>();

            //    _gameInfos.Add(gameInfo);
            //}

            Log("", "Getting BotAuthenticationGuid from registry...");

			using (var view32 = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine,
                                                RegistryView.Registry32))
            {
                using (var key = view32.OpenSubKey(@"SOFTWARE\Microsoft\Microsoft Games\Allegiance\1.4\Server"))
                {
                    _botAuthenticationGuid = (string)key.GetValue("BotAuthenticationGuid");
                }
            }

            if (String.IsNullOrEmpty(_botAuthenticationGuid) == true)
                throw new Exception("Couldn't read SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\1.4\\Server\\BotAuthenticationGuid !! Ensure this value is set.");
			

            //LoadStrategies();

            //_messagePump = Task.Run(() =>
            //{
            //    SendAndReceiveMessagesForAllClients(cancellationTokenSource);

                
            //});
        }
        
        public void DisconnectAllClients()
        {
            lock (_connectedClientsByPlayerName)
            {
                

                foreach (var runningStrategy in _currentStrategyByPlayerName.Values)
                {
                    runningStrategy.Cancel();
                }

                //foreach (var connection in _connectedClientsByPlayerName.Values)
                //{
                //    connection.DisconnectLobby();
                //    connection.DisconnectServer();
                //}

                _connectedClientsByPlayerName.Values.AsParallel().ForAll((AllegianceInterop.ClientConnection connection) =>
                {
                    connection.DisconnectLobby();
                    connection.DisconnectServer();
                });
            }
        }

        public void SendAndReceiveMessagesForAllClients(/*CancellationTokenSource cancellationTokenSource*/)
        {
            //while (cancellationTokenSource.IsCancellationRequested == false)
            //{
                lock (_connectedClientsByPlayerName)
                {
                    _connectedClientsByPlayerName.Values.AsParallel().ForAll((AllegianceInterop.ClientConnection connection) =>
                    {
                        connection.SendAndReceiveUpdate();
                    });

                    //foreach (var connection in _connectedClientsByPlayerName.Values)
                    //    connection.SendAndReceiveUpdate();

                }

                foreach (var strategy in _currentStrategyByPlayerName.Values.ToArray())
                {
                    //if (strategy.IsStrategyComplete == true)
                    //    StrategyComplete(strategy);

                    if (DateTime.Now - strategy.StartTime > strategy.Timeout)
                        ResetClient(strategy.PlayerName);
                    
                }

                //Thread.Sleep(10);
            //}
        }

        private void ResetClient(string playerName)
        {
            StrategyBase currentStrategy;
            if (_currentStrategyByPlayerName.TryGetValue(playerName, out currentStrategy) == false)
                currentStrategy = null;
            
            AllegianceInterop.ClientConnection client;
            if (_connectedClientsByPlayerName.TryGetValue(playerName, out client) == true)
            {
                client.DisconnectServer();
                client.DisconnectLobby();

                //client.OnAppMessage -= _currentStrategyAppMessageDelegateByPlayerName[playerName];

                _connectedClientsByPlayerName.Remove(playerName);
                //_currentStrategyAppMessageDelegateByPlayerName.Remove(playerName);
                _currentStrategyByPlayerName.Remove(playerName);

            }

            Log(playerName, "Resetting player after strategy timeout.");

            CreatePlayer(playerName, currentStrategy.SideIndex, currentStrategy.IsGameController, currentStrategy.IsCommander);
        }

        internal void UpdateTeamStrategies()
        {
            // Get team leader's ship
            var teamLeaderShip = _connectedClientsByPlayerName.Values.ToArray()
                .Where(p => p.GetShip() != null && p.GetSide() != null &&  p.SideLeaderShipID(p.GetSide().GetObjectID()) == p.GetShip()?.GetObjectID()).FirstOrDefault()?.GetShip();

            if (teamLeaderShip == null)
                return;

            if (_gameInfo.Clusters == null)
                return;

            // Wait for the map to be fully scouted before starting combat operations.
            if (_gameInfo.GetUnexploredClusters(teamLeaderShip.GetMission()).Count > 0 || _gameInfo.Clusters.Count == 0)
                return;

            // Do we have any enemy constructors visible?
        }

        //public void LoadStrategies()
        //{
        //    // To load strategies into the current app domain without using transparent proxy, you can uncomment this, but it
        //    // will break dynamic strategy updates. 
        //    //
        //    //_currentAppDomain = AppDomain.CurrentDomain;
        //    //string target = Path.Combine(_currentAppDomain.BaseDirectory, "Strategies.dll");
        //    //_assemblyLoader = new AssemblyLoader(target);
        //    //return;

        //    // Load strategies into a separate app domain so that they can be hot-swapped while a game is running. 
        //    var an = Assembly.GetExecutingAssembly().GetName();

        //    var appDomainSetup = new AppDomainSetup
        //    {
        //        PrivateBinPath = AppDomain.CurrentDomain.BaseDirectory,
        //        ShadowCopyDirectories = AppDomain.CurrentDomain.BaseDirectory,
        //        ShadowCopyFiles = "true"
        //    };

        //    _currentAppDomain = AppDomain.CreateDomain("V1", (Evidence)null, appDomainSetup);

        //    //Console.WriteLine(_currentAppDomain.BaseDirectory);

        //    _currentAppDomain.Load(typeof(AssemblyLoader).Assembly.FullName);

        //    string target = Path.Combine(_currentAppDomain.BaseDirectory, "Strategies.dll");

        //    _assemblyLoader = (AssemblyLoader)Activator.CreateInstance(
        //        _currentAppDomain,
        //        typeof(AssemblyLoader).Assembly.FullName,
        //        typeof(AssemblyLoader).FullName,
        //        false,
        //        BindingFlags.Public | BindingFlags.Instance,
        //        null,
        //        new object[] { target },
        //        null,
        //        null).Unwrap();
        //}

        public void ChangeStrategy(AllegianceInterop.ClientConnection clientConnection, StrategyID strategyID, StrategyBase lastStrategy)
        {
            ChangeStrategy(clientConnection, strategyID, lastStrategy.PlayerName, lastStrategy.SideIndex, lastStrategy.IsGameController, lastStrategy.IsCommander);
        }

        internal void PostResignAndQuit()
        {
            var firstConnectedClient = _connectedClientsByPlayerName.Values.FirstOrDefault();

            var ship = firstConnectedClient.GetShip();

            if (ship != null)    
                firstConnectedClient.SendChat(ship, AllegianceInterop.ChatTarget.CHAT_TEAM, -1, -1, "#resign", -1, -1, -1, null, false);
        }

        public void ChangeStrategy(AllegianceInterop.ClientConnection clientConnection, StrategyID strategyID, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            //AllegianceInterop.ClientConnection clientConnection;

            //if (_connectedClientsByPlayerName.TryGetValue(playerName, out clientConnection) == false)
            //{
            //    throw new Exception("Couldn't get connection for playerName: " + playerName);
            //}

            //AllegianceInterop.ClientConnection.OnAppMessageDelegate currentStrategyOnAppMessageDelegate;
            //if (_currentStrategyAppMessageDelegateByPlayerName.TryGetValue(playerName, out currentStrategyOnAppMessageDelegate) == true)
            //{
            //    clientConnection.OnAppMessage -= currentStrategyOnAppMessageDelegate;
            //    _currentStrategyAppMessageDelegateByPlayerName.Remove(playerName);
            //}

            StrategyBase currentStrategy;
            if (_currentStrategyByPlayerName.TryGetValue(playerName, out currentStrategy) == false)
            {
                currentStrategy = null;
            }
            else
            {
                //currentStrategy.OnStrategyComplete -= Strategy_OnStrategyComplete;
                _currentStrategyByPlayerName.Remove(playerName);
            }

            StrategyBase strategy = StrategyBase.GetInstance(strategyID, clientConnection, _gameInfo, _botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            strategy.OnStrategyComplete += Strategy_OnStrategyComplete;
            strategy.OnGameComplete += Strategy_OnGameComplete;


            //StrategyBase strategy = _assemblyLoader.CreateInstance(strategyID);

            //strategy.Attach(clientConnection, _gameInfos[sideIndex], _botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);




            // Can't hook the transparent object directly to the client, the events raised by c++/cli won't trigger on the managed end
            // becuase they are attached to a copy of the object in the far application domain, so we will attach to the event on the 
            // near application domain, then directly call the MessageReceiver handler in the far domain passing the byte array 
            // through instead of the tranparent proxied object.
            //AllegianceInterop.ClientConnection.OnAppMessageDelegate onAppMessageDelegate = new AllegianceInterop.ClientConnection.OnAppMessageDelegate((AllegianceInterop.ClientConnection messageClientConnection, byte[] bytes) =>
            //    {
            //        strategy.OnAppMessage(bytes);
            //    });

            //    clientConnection.OnAppMessage += onAppMessageDelegate;

            //    _currentStrategyAppMessageDelegateByPlayerName.Add(playerName, onAppMessageDelegate);
            //    _currentStrategyByPlayerName.Add(playerName, strategy);


            //}
            //else
            //{
            //    throw new Exception($"Error, couldn't find the {strategyID.ToString()} strategy in the loaded strategy list.");
            //}

        }

        private void Strategy_OnGameComplete(StrategyBase strategy)
        {
            Log(strategy.PlayerName, $"OnGameComplete: Exiting WOPR to trigger a new game to start.");

            _cancellationTokenSource.Cancel();
        }

        private class CreatePlayerThreadParams
        {
            public string playerName;
            public short sideIndex;
            public bool isGameController;
            public bool isCommander;
        }

        private void CreatePlayerThread(object createPlayerObject)
        {
            CreatePlayerThreadParams createPlayerParams = (CreatePlayerThreadParams)createPlayerObject;

            Console.WriteLine($"Creating player: {createPlayerParams.playerName}");


            //if (_teamDirectorPlayerInfoByPlayerName.ContainsKey(playerName) == true)
            //    _teamDirectorPlayerInfoByPlayerName.Remove(playerName);

            //_teamDirectorPlayerInfoByPlayerName.Add(playerName, new TeamDirectorPlayerInfo()
            //{
            //    IsCommander = isCommander,
            //    IsGameController = isGameController,
            //    PlayerName = playerName, 
            //    SideIndex = sideIndex
            //});


            AllegianceInterop.ClientConnection clientConnection = new AllegianceInterop.ClientConnection(Configuration.ArtPath);



            //StrategyBase strategy;
            //if (_strategiesByStrategyID.TryGetValue(StrategyID.ConnectToGame, out strategy) == true)
            //{
            //    strategy.Attach(clientConnection, _botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            //    strategy.OnStrategyComplete += Strategy_OnConnectToGameStrategyComplete;

            //    // Can't hook the transparent object directly to the client, the events raised by c++/cli won't trigger on the managed end
            //    // becuase they are attached to a copy of the object in the far application domain, so we will attach to the event on the 
            //    // near application domain, then directly call the MessageReceiver handler in the far domain passing the byte array 
            //    // through instead of the tranparent proxied object.
            //    clientConnection.OnAppMessage += new AllegianceInterop.ClientConnection.OnAppMessageDelegate((AllegianceInterop.ClientConnection messageClientConnection, byte[] bytes) =>
            //    {
            //        strategy.OnAppMessage(bytes);
            //    });
            //}
            //else
            //{
            //    throw new Exception("Error, couldn't find the ConnectToGame strategy in the loaded strategy list.");
            //}

            ChangeStrategy(clientConnection, StrategyID.ConnectToGame, createPlayerParams.playerName, createPlayerParams.sideIndex, createPlayerParams.isGameController, createPlayerParams.isCommander);

            bool connected = false;
            for (int i = 0; i < 30; i++)
            {
                if (clientConnection.ConnectToLobby(_cancellationTokenSource, _lobbyAddress, createPlayerParams.playerName, _botAuthenticationGuid) == true)
                {
                    lock (_connectedClientsByPlayerName)
                    {
                        _connectedClientsByPlayerName.Add(createPlayerParams.playerName, clientConnection);
                    }

                    connected = true;
                    break;
                }

                Log(createPlayerParams.playerName, "Couldn't connect, retrying.");

                Thread.Sleep(100);
            }

            if (connected == false)
            {
                Log(createPlayerParams.playerName, "Couldn't connect. Giving up!");
            }
            else
            {
                Log(createPlayerParams.playerName, "Starting message processing loop.");

                //Task.Run(() =>
                //{
                    while (_cancellationTokenSource.IsCancellationRequested == false)
                    {
                        try
                        {
                            clientConnection.SendAndReceiveUpdate();
                        }
                        catch (Exception ex)
                        {
                            Log(createPlayerParams.playerName, ex);
                        }

                        StrategyBase currentStrategy;
                        if (_currentStrategyByPlayerName.TryGetValue(createPlayerParams.playerName, out currentStrategy) == true)
                        {
                            if (DateTime.Now - currentStrategy.StartTime > currentStrategy.Timeout)
                            {
                                ResetClient(currentStrategy.PlayerName);
                                break;
                            }
                        }

                        Thread.Sleep(100);
                    }
                //});
            }
        }



        public void CreatePlayer(string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            Thread thread = new Thread(new ParameterizedThreadStart(CreatePlayerThread));
            thread.Start(new CreatePlayerThreadParams()
            {
                playerName = playerName,
                sideIndex = sideIndex,
                isGameController = isGameController,
                isCommander = isCommander
            });
        }

        private void Strategy_OnStrategyComplete(StrategyBase strategy)
        {
            Console.WriteLine($"TeamDirector {strategy.PlayerName}: {strategy.StrategyID} has completed.");

            if (strategy.IsCommander == true)
            {
                switch (strategy.StrategyID)
                {
                    case StrategyID.ConnectToGame:
                        ChangeStrategy(strategy.ClientConnection, StrategyID.CommanderResearchAndExpand, strategy.PlayerName, strategy.SideIndex, strategy.IsGameController, strategy.IsCommander);
                        break;

                    default:
                        Console.WriteLine($"TeamDirector {strategy.PlayerName}: couldn't determine next strategy from the current strategy. (Should probably set a default here?)");
                        break;

                }
            }
            else // Pilot Strategies.
            {
                switch (strategy.StrategyID)
                {
                    case StrategyID.ConnectToGame:
                        ChangeStrategy(strategy.ClientConnection, StrategyID.ScoutExploreMap, strategy.PlayerName, strategy.SideIndex, strategy.IsGameController, strategy.IsCommander);
                        break;

                    default:
                        Console.WriteLine($"TeamDirector {strategy.PlayerName}: couldn't determine next strategy from the current strategy. (Should probably set a default here?)");
                        break;

                }
            }

        }

        private void Log(string playerName, string message)
        {
            File.AppendAllText(@"c:\1\Logs\" + playerName + "_teamdirector.txt", DateTime.Now.ToString() + ": " + message + "\n");
        }

        private void Log(string playerName, Exception ex)
        {
            string message = "An exception ocurred in SendAndReceiveUpdate: \n" + ex.ToString();

            Log(playerName, message);
            File.AppendAllText(@"c:\1\Logs\_exceptions.txt", DateTime.Now.ToString() + ": " + message + "\n");
        }


        //private void clientConnection_OnAppMessage(AllegianceInterop.ClientConnection clientConnection, byte[] bytes)
        //{
        //    _currentStrategies.AsParallel().ForAll((StrategyBase strategy) =>
        //    {
        //        strategy.OnAppMessage(bytes);
        //    });

        //}
    }
}
