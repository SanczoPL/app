/***************************************************************************
 SocNetV: Social Network Visualizer
 version: 2.5
 Written in Qt
 
                         graph.h  -  description
                             -------------------
    copyright         : (C) 2005-2019 by Dimitris B. Kalamaras
    project site      : https://socnetv.org

 ***************************************************************************/

/*******************************************************************************
*     This program is free software: you can redistribute it and/or modify     *
*     it under the terms of the GNU General Public License as published by     *
*     the Free Software Foundation, either version 3 of the License, or        *
*     (at your option) any later version.                                      *
*                                                                              *
*     This program is distributed in the hope that it will be useful,          *
*     but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*     GNU General Public License for more details.                             *
*                                                                              *
*     You should have received a copy of the GNU General Public License        *
*     along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
********************************************************************************/

#ifndef GRAPH_H
#define GRAPH_H


#include <QObject>
#include <QDateTime> 	// used in exporting centrality files
#include <QList>
#include <QHash>
#include <QTextStream>
#include <QThread>

#include <QtCharts/QChartGlobal>
#include <QAbstractSeries>

//FYI: stack is a wrapper around <deque> in C++, see: www.cplusplus.com/reference/stl/stack
#include <stack>
#include <map>

#include "global.h"
#include "graphvertex.h"
#include "matrix.h"
#include "parser.h"
#include "webcrawler.h"
#include "graphicswidget.h"

QT_BEGIN_NAMESPACE
class QPointF;
QT_END_NAMESPACE


QT_CHARTS_BEGIN_NAMESPACE
class QAbstractSeries;
class QAbstractAxis;
class QSplineSeries;
class QBarSeries;
class QAreaSeries;
class QBarSet;
class QBarCategoryAxis;
QT_CHARTS_END_NAMESPACE



QT_CHARTS_USE_NAMESPACE

SOCNETV_USE_NAMESPACE


class Chart;

using namespace std;



typedef QList<GraphVertex*> VList;
typedef QHash <QString, int> H_StrToInt;
typedef QHash <int, int> H_Int;
typedef QHash <qreal, int> H_f_i;
typedef QPair <qreal, bool> pair_f_b;
typedef QPair <int, pair_f_b > pair_i_fb;
typedef QHash <int, pair_i_fb > H_edges;
typedef QHash<QString, bool> H_StrToBool;
typedef QList<int> L_int;
typedef QVector<int> V_int;
typedef QVector<QString> V_str;





/**
  TODO & KNOWN BUGS:
  \todo Group edge editing: i.e. change weight or color.
  \todo - CHECK weighted networks results (IRCC and distance matrix with other combinations)
  \todo - CHECK graphIsWeighted corner case results, when !graphIsModified.

  \bug Create d-regular, undirected, ask for closeness, it says we are on a disconnected graph
  \bug Crash on Graphml files with textlabels instead of nodenumbers (i.e. nets/killer.graphml)
  \bug wontfix Crash on Graphml files with html special chars in node/edge labels
  \bug wontfix Pajek files with no ic / labels without quotes are displayed without colors
  \bug wrong default edge colors (not the ones used by Settings) after loading GraphML files.
  \bug Resizing the MW view does not resize/reposition the layout guides



  TODO
  Change Menus to:
  Matrices
  Cohesion/Connectedness: Density, Reachability, and All distance and Walks, Connectivity, Reciprocity, Transitivity ?, Clu Cof
  Prominence: Centrality and Prestige
  Subgroups / Communities: Cliques (later clans etc), (later path distance MDS) Components, Blocks and Cutpoints,
  Structural Equivalence: HCA, Similarity (later MDS), Block modelling, CONCOR

  */



/**
 * @brief The Graph class
 * This is the main class for a Graph, used in conjuction with GraphVertex, Parser and Matrix objects.
 *   Graph class methods are the interface to various analysis algorithms
 *   GraphVertex class holds each vertex data (colors, strings, statistics, etc)
 *   Matrix class holds the adjacency matrix of the network.
 *   Parser class loads files of networks.
 */
class Graph:  public QObject{
    Q_OBJECT
    QThread file_parserThread;
    QThread wc_parserThread;
    QThread wc_spiderThread;

public slots:

    int relationCurrent();

    QString relationCurrentName() const;

    void relationCurrentRename(const QString &newName, const bool &notifyMW=false);

    /** Slots to signals from Parser */
    void vertexCreate (const int &number,
                        const int &size,
                        const QString &color,
                        const QString &numColor,
                        const int &numSize,
                        const QString &label,
                        const QString &labelColor,
                        const int &labelSize,
                        const QPointF &p,
                        const QString &shape,
                        const QString &iconPath = QString::null,
                        const bool &signalMW = false
            );

    void graphFileLoaded(const int &fileType,
                         const QString &fName=QString::null,
                         const QString &netName=QString::null,
                         const int &totalNodes=0,
                         const int &totalLinks=0,
                         const int &edgeDirType=0,
                         const QString &message=QString::null);

    void vertexRemoveDummyNode(int);

    void graphLoadedTerminateParserThreads (QString reason);

    void graphSelectionChanged(const QList<int> selectedVertices,
                               const QList<SelectedEdge> selectedEdges);


    /** Slots to signals from GraphicsWidget and Parser*/
    void edgeCreate  (const int &v1, const int &v2, const qreal &weight,
                      const QString &color ,
                      const int &type=0,
                      const bool &drawArrows=true, const bool &bezier=false,
                      const QString &label=QString::null,
                      const bool &signalMW=true);

    void edgeCreateWebCrawler (const int &source, const int &target);

    void edgeVisibilitySet(int relation, int, int, bool);

    //auxiliary vertexCreate functions
    void vertexCreateAtPos(const QPointF &p);

    void vertexCreateAtPosRandom(const bool &signalMW=false);

    void vertexCreateAtPosRandomWithLabel(const int &i,
                                          const QString &label,
                                          const bool &signalMW=false) ;


    /** Slots to signals from MainWindow */

    void relationSet(int relNum=RAND_MAX, const bool notifyMW=true);

    void relationNext();

    void relationPrev();

    void canvasSizeSet(const int w, const int h);

    double canvasMaxRadius() const;

    qreal canvasMinDimension() const;

    double canvasVisibleX(const double &x) const ;

    double canvasVisibleY(const double &y) const ;

    double canvasRandomX()  const;

    double canvasRandomY()  const;


    void vertexIsolatedAllToggle ( const bool &toggle);

    void vertexClickedSet(const int &v);

    void edgeClickedSet(const int &v1, const int &v2, const bool &openMenu=false) ;


    void edgeFilterByWeight (qreal, bool);

    void edgeFilterByRelation(int relation, bool status);

    void edgeFilterUnilateral(const bool &toggle);

    void webCrawl(const QString &seedUrl,
                  const QStringList &urlPatternsIncluded,
                  const QStringList &urlPatternsExcluded,
                  const QStringList &linkClasses,
                  const int &maxNodes,
                  const int &maxLinksPerPage,
                  const bool &intLinks,
                  const bool &childLinks,
                  const bool &parentLinks,
                  const bool &selfLinks,
                  const bool &extLinksIncluded,
                  const bool &extLinksCrawl,
                  const bool &socialLinks,
                  const bool &delayedRequests);

    QString htmlEscaped (QString str) const;


signals:
    /** Signals to MainWindow */
    void signalProgressBoxCreate(const int max=0, const QString msg="Please wait");

    void signalProgressBoxKill(const int max=0);

    void signalProgressBoxUpdate(const int &count=0 );

    void signalGraphSavedStatus(const int &status);

    void signalGraphModified(const bool &undirected,
                             const int &vertices,
                             const int &edges,
                             const qreal &density);

    void signalGraphLoaded (const int &fileType,
                            const QString &fileName=QString::null,
                            const QString &netName=QString::null,
                            const int &totalNodes=0,
                            const int &totalLinks=0,
                            const QString &message=QString::null );



    void statusMessage (const QString &message);

    void signalDatasetDescription(QString);

    void signalNodeClickedInfo(const int &number=0,
                                    const QPointF &p=QPointF(),
                                    const QString &label=QString::null,
                                    const int &inDegree=0,
                                    const int &outDegree=0,
                                    const qreal &clc=0);

//    void signalEdgeClicked (const int &v1=0,
//                            const int &v2=0,
//                            const qreal &weight=0,
//                            const int &type=0,
//                            const bool &openMenu=false);

    void signalEdgeClicked (const MyEdge &edge=MyEdge(),
                            const bool &openMenu=false);

    void signalRelationAddToMW(const QString &newRelation,
                               const bool &changeRelation=true);

    void signalRelationsClear();

    void signalRelationRenamedToMW(const QString newRelName);

    void signalRelationChangedToGW(int);

    void signalRelationChangedToMW(const int &relIndex=RAND_MAX);

    void signalSelectionChanged(const int &selectedVertices,
                                const int &selectedEdges);


    void signalPromininenceDistributionChartUpdate(QAbstractSeries *series,
                                                   QAbstractAxis *axisX=Q_NULLPTR,
                                                   const qreal &min=0,
                                                   const qreal &max=0,
                                                   QAbstractAxis *axisY=Q_NULLPTR,
                                                   const qreal &minF=0,
                                                   const qreal &maxF=0);

    /** Signals to GraphicsWidget */
    void signalDrawNode( const QPointF &p,
                         const int &num,
                         const int &size,
                         const QString &nodeShape,
                         const QString &nodeIconPath,
                         const QString &nodeColor,
                         const QString &numberColor, const int &numSize,
                         const int &numDistance,
                         const QString &label,
                         const QString &labelColor, const int &labelSize,
                         const int &labelDistance
                         );

    //signal to GW to erase a node
    void signalRemoveNode (int );

    //signal GW to draw an edge
    void signalDrawEdge ( const int &v1,
                          const int &v2,
                          const qreal &weight,
                          const QString &label="",
                          const QString &color="black",
                          const int &type=0,
                          const bool &drawArrows=true,
                          const bool &bezier=false,
                          const bool &weightNumbers=false);

    //signal to GW
    void signalRemoveEdge(const int &v1, const int &v2, const bool &removeOpposite);

    void setEdgeVisibility (int, int, int, bool);

    void setVertexVisibility(int, bool);

    void setNodePos(const int &, const qreal &, const qreal &);

    void signalNodesFound(const QList<int> foundList);

    void setNodeSize(const int &v, const int &size);

    void setNodeShape(const int &v, const QString &shape, const QString &iconPath=QString::null);

    void setNodeColor(const int &v, const QString &color);

    void setNodeLabel(const int &v, const QString &label);

    void setNodeNumberColor(const int &v, const QString &color);

    void setNodeNumberSize(const int &v, const int &size);

    void setNodeNumberDistance(const int &v, const int &distance);

    void setNodeLabelSize(const int &v, const int &size);

    void setNodeLabelColor(const int &v, const QString &color);

    void setNodeLabelDistance(const int &v, const int &distance);


    void setEdgeWeight (const int &v1, const int &v2, const qreal &weight);
    void signalEdgeType(const int &v1,
                        const int &v2,
                        const int &type);
    void setEdgeColor(const int &v1,
                         const int &v2,
                         const QString &color);
    void setEdgeLabel (const int &v1,
                       const int &v2,
                       const QString &label);
    void addGuideCircle(const double&, const double&, const double&);
    void addGuideHLine (const double&y0);



    /** Signals to Crawler threads */
    void  operateSpider();


public:

    enum GraphChange {
        ChangedNone                = 0,
        ChangedMinorOptions        = 1,
        ChangedVerticesMetadata    = 2,
        ChangedEdgesMetadata       = 3,
        ChangedPositions           = 4,
        ChangedMajor               = 10,
        ChangedVertices            = 11,
        ChangedEdges               = 12,
        ChangedVerticesEdges       = 13,
        ChangedNew                 = 14,
    };



    enum Clustering {
        Single_Linkage   = 0, //"single-link" or minimum
        Complete_Linkage = 1, // "complete-link or maximum
        Average_Linkage  = 2, //mean or "average-linkage" or UPGMA

    };

    /* INIT AND CLEAR*/
    Graph(GraphicsWidget *graphicsWidget);
    void clear(const QString &reason="");
        ~Graph();

    void setSocNetV_Version (QString ver) { VERSION = ver; }

    GraphicsWidget *canvas() { return m_canvas; }


    /*FILES (READ AND WRITE)*/
    QString graphName() const;

    void graphLoad (const QString m_fileName,
                    const QString m_codecName,
                    const int format,
                    const int two_sm_mode,
                    const QString delimiter=QString::null);

    void graphSave(const QString &fileName,
                   const int &fileType,
                   const bool &saveEdgeWeights=true);

    bool graphSaveToPajekFormat (const QString &fileName,
                                 QString networkName="",
                                 int maxWidth=0, int maxHeight=0);

    bool graphSaveToAdjacencyFormat (const QString &fileName,
                                     const bool &saveEdgeWeights=true);

    bool graphSaveToGraphMLFormat (const QString &fileName,
                                   QString networkName="",
                                   int maxWidth=0, int maxHeight=0);

    bool graphSaveToDotFormat (QString fileName);

    int graphFileFormat() const;

    bool graphFileFormatExportSupported(const int &fileFormat) const;

    QString graphMatrixTypeToString(const int &matrixType) const;

    int graphMatrixStrToType(const QString &matrix) const;

    QString graphMetricTypeToString(const int &metricType) const;

    int graphMetricStrToType(const QString &metricStr) const;

    QString graphClusteringMethodTypeToString(const int &methodType) const;

    int graphClusteringMethodStrToType(const QString &method) const;


    /* RELATIONS */
    int relations();

    void relationsClear();

    void relationAdd(const QString &relName, const bool &changeRelation=false);


    /* VERTICES */
    int vertexNumberMax();

    int vertexNumberMin();

    int vertexDegreeOut(int);

    int vertexDegreeIn(int);

    QList<int> vertexNeighborhoodList(const int &v1);

    bool vertexIsolated(const int &v1) const;

    int vertexExists(const int &v1 );

    int vertexExists(const QString &label);

    bool vertexFindByNumber (const QStringList &numList);

    bool vertexFindByLabel (const QStringList &labelList);

    bool vertexFindByIndexScore(const int &index, const QStringList &thresholds);

    void vertexRemove (const int &v1);

    void vertexSizeInit (const int);

    void vertexSizeSet(const int &v, const int &newsize);

    int vertexSize(const int &v) const;

    void vertexShapeSetDefault (const QString, const QString &iconPath=QString::null);

    void vertexShapeSet(const int &v, const QString &shape, const QString &iconPath=QString::null);

    QString vertexShape(const int &v);

    QString vertexShapeIconPath(const int &v);

    bool graphHasVertexCustomIcons () const { return m_graphHasVertexCustomIcons; }

    void vertexColorInit (const QString &color);

    void vertexColorSet(const int &v, const QString &color);

    QColor vertexColor(const int &v) const;

    void vertexNumberColorInit (const QString &color);

    void vertexNumberColorSet(const int &v=0, const QString &color = "#000000" );

    void vertexNumberSizeInit (const int &size);

    void vertexNumberSizeSet(const int &v, const int &newsize );

    void vertexNumberDistanceInit (const int &distance);

    void vertexNumberDistanceSet(const int &v, const int &newDistance );

    void vertexLabelSet(const int &v, const QString &label);

    QString vertexLabel(const int &v) const;

    void vertexLabelsVisibilitySet(bool toggle);

    void vertexLabelSizeInit(int newSize);

    void vertexLabelSizeSet(const int &v, const int &labelSize );

    void vertexLabelColorInit(QString color);

    void vertexLabelColorSet(const int &v1, const QString &color);

    void vertexLabelDistanceInit (const int &distance);

    void vertexLabelDistanceSet(const int &v, const int &newDistance );

    void vertexLabelDistanceAllSet (const int &newDistance);


    void vertexPosSet(const int &v, const int &x, const int &y);

    QPointF vertexPos(const int &v1) const;

    int vertexClicked() const;

    int vertices(const bool &dropIsolates=false, const bool &countAll=false, const bool &recount=false) ;

    int vertexEdgesOutbound (int i) ;

    int vertexEdgesInbound (int i) ;


    int verticesWithOutboundEdges();

    int verticesWithInboundEdges();

    int verticesWithReciprocalEdges();


    QList<int> verticesListIsolated();

    QList<int> verticesList();

    QSet<int> verticesSet();



    void verticesCreateSubgraph(QList<int> vList,
                                const int &type = SUBGRAPH_CLIQUE,
                                const int &center = 0);




    /* EDGES */

    int edgesEnabled();

    MyEdge edgeClicked();

    qreal edgeExists(const int &v1,
                     const int &v2,
                     const bool &checkReciprocal=false);

    void edgeRemove (const int &v1,
                     const int &v2,
                     const bool &removeOpposite=false);

    void edgeRemoveSelected (SelectedEdge &selectedEdge,
                             const bool &removeOpposite);

    void edgeRemoveSelectedAll();

    bool edgeSymmetric(const int &v1, const int &v2);

    void edgeTypeSet(const int &v1,
                     const int &v2,
                     const qreal &w,
                     const int &dirType=EdgeType::Directed);

    void edgeWeightSet (const int &v1,
                        const int &v2,
                        const qreal &w,
                        const bool &undirected=false);

    qreal edgeWeight(const int &v1, const int &v2) const;

    void edgeWeightNumbersVisibilitySet (const bool &toggle);

    void edgeLabelSet(const int &v1, const int &v2, const QString &label);

    QString edgeLabel (const int &v1, const int &v2) const;

    void edgeLabelsVisibilitySet (const bool &toggle);

    void edgeColorInit(const QString &);

    void edgeColorSet(const int &v1, const int &v2, const QString &color);

    QString edgeColor (const int &v1, const int &v2);

    bool edgeColorAllSet(const QString &color, const int &threshold=RAND_MAX);


    /* GRAPH methods */

    void graphSetModified(const int &graphNewStatus, const bool&signalMW=true);

    bool graphIsModified() const ;

    bool graphSaved() const;

    bool graphLoaded() const;

    QList<int> graphSelectedVertices() const;

    int graphSelectedVerticesCount() const;

    int graphSelectedVerticesMin() const;

    int graphSelectedVerticesMax() const;

    QList<SelectedEdge> graphSelectedEdges() const;

    int graphSelectedEdgesCount() const;

    int graphGeodesics();

    qreal graphDensity();

    bool graphIsWeighted();

    void graphSetWeighted(const bool &toggle=true);

    qreal graphReciprocity();

    bool graphIsSymmetric();

    void graphSymmetrize();

    void graphSymmetrizeStrongTies(const bool &allRelations=false);

    void graphCocitation();

    void graphDichotomization(const qreal threshold);

    void graphSetDirected(const bool &toggle=true, const bool &signalMW=true);

    void graphSetUndirected(const bool &toggle=true, const bool &signalMW=true);

    bool graphIsDirected();

    bool graphIsUndirected();

    bool graphIsConnected();

    void graphMatrixAdjacencyCreate(const bool dropIsolates=false,
                                    const bool considerWeights=true,
                                    const bool inverseWeights=false,
                                    const bool symmetrize=false );

    bool graphMatrixAdjacencyInvert(const QString &method="lu");


    void graphMatrixSimilarityMatchingCreate(Matrix &AM,
                                             Matrix &SEM,
                                             const int &measure=METRIC_SIMPLE_MATCHING,
                                             const QString &varLocation="Rows",
                                             const bool &diagonal=false,
                                             const bool &considerWeights=true);

    void graphMatrixSimilarityPearsonCreate (Matrix &AM,
                                             Matrix &PCC,
                                             const QString &varLocation="Rows",
                                             const bool &diagonal=false);

    void graphMatrixDissimilaritiesCreate(Matrix &INPUT_MATRIX,
                                          Matrix &DSM,
                                          const int &metric,
                                          const QString &varLocation,
                                          const bool &diagonal,
                                          const bool &considerWeights);

    /* REPORT EXPORTS */
    void setReportsDataDir(const QString &reportsDir);
    void setReportsRealNumberPrecision (const int & precision);
    void setReportsLabelLength(const int &length);
    void setReportsChartType(const int &type);

    void writeDataSetToFile(const QString dir, const QString );

    void writeMatrixAdjacencyTo(QTextStream& os,
                                const bool &saveEdgeWeights=true);

    void writeReciprocity( const QString fileName,
                           const bool considerWeights=false);

    void writeMatrix(const QString &fileName,
                     const int &matrix=MATRIX_ADJACENCY,
                     const bool &considerWeights=true,
                     const bool &inverseWeights=false,
                     const bool &dropIsolates=false,
                     const QString &varLocation="Rows",
                     const bool &simpler=false);

    void writeMatrixHTMLTable(QTextStream &outText, Matrix &M,
                              const bool &markDiag=true,
                              const bool &plain=false,
                              const bool &printInfinity=true,
                              const bool &dropIsolates=false);

    void writeMatrixAdjacency(const QString fileName,
                              const bool &markDiag=true);

    void writeMatrixAdjacencyPlot(const QString fileName,
                                      const bool &simpler=false);

    void writeMatrixAdjacencyInvert(const QString &filename,
                                    const QString &method);

    void writeMatrixLaplacianPlainText(const QString &filename);
    void writeMatrixDegreeText(const QString &filename);

    void writeMatrixDistancesPlainText(const QString &fn,
                                       const bool &considerWeights,
                                       const bool &inverseWeights,
                                       const bool &dropIsolates);

    void writeMatrixShortestPathsPlainText(const QString &fn,
                                               const bool &considerWeights,
                                               const bool &inverseWeights);

    void writeMatrixDissimilarities(const QString fileName,
                                    const QString &metricStr,
                                    const QString &varLocation,
                                    const bool &diagonal,
                                    const bool &considerWeights) ;

    void writeMatrixSimilarityMatchingPlain(const QString fileName,
                                            const int &measure=METRIC_SIMPLE_MATCHING,
                                            const QString &matrix = "adjacency",
                                            const QString &varLocation="rows",
                                            const bool &diagonal=false,
                                            const bool &considerWeights=true);

    void writeMatrixSimilarityMatching(const QString fileName,
                                       const QString &measure="Simple",
                                       const QString &matrix = "adjacency",
                                       const QString &varLocation="rows",
                                       const bool &diagonal=false,
                                       const bool &considerWeights=true);


    void writeMatrixSimilarityPearson(const QString fileName,
                                      const bool considerWeights,
                                      const QString &matrix = "adjacency",
                                      const QString &varLocation="rows",
                                      const bool &diagonal=false);

    void writeMatrixSimilarityPearsonPlainText(const QString fileName,
                                               const bool considerWeights,
                                               const QString &matrix = "adjacency",
                                               const QString &varLocation="rows",
                                               const bool &diagonal=false);

    void writeEccentricity( const QString fileName,
                            const bool considerWeights=false,
                            const bool inverseWeights=false,
                            const bool dropIsolates=false);

 //   friend QTextStream& operator <<  (QTextStream& os, Graph& m);

    void writeCentralityDegree(const QString,
                               const bool weights,
                               const bool dropIsolates);

    void writeCentralityCloseness(const QString,
                                  const bool weights,
                                  const bool inverseWeights,
                                  const bool dropIsolates);

    void writeCentralityClosenessInfluenceRange(const QString,
                                                const bool weights,
                                                const bool inverseWeights,
                                                const bool dropIsolates);

    void writeCentralityBetweenness(const QString,
                                    const bool weights,
                                    const bool inverseWeights,
                                    const bool dropIsolates);

    void writeCentralityPower(const QString,
                              const bool weigths,
                              const bool inverseWeights,
                              const bool dropIsolates);

    void writeCentralityStress(const QString,
                               const bool weigths,
                               const bool inverseWeights,
                               const bool dropIsolates);

    void writeCentralityEccentricity(const QString,
                                     const bool weigths,
                                     const bool inverseWeights,
                                     const bool dropIsolates);

    void writeCentralityInformation(const QString,
                                    const bool weigths,
                                    const bool inverseWeights);

    void writeCentralityEigenvector(const QString,
                                    const bool &weigths=true,
                                    const bool &inverseWeights = false,
                                    const bool &dropIsolates=false);

    void writePrestigeDegree(const QString, const bool weights,
                             const bool dropIsolates);

    void writePrestigeProximity(const QString, const bool weights,
                                const bool inverseWeights,
                                const bool dropIsolates);

    void writePrestigePageRank(const QString, const bool Isolates=false);


    bool writeClusteringHierarchical(const QString &fileName,
                                     const QString &varLocation,
                                     const QString &matrix = "Adjacency",
                                     const QString &metric = "Manhattan",
                                     const QString &method = "Complete",
                                     const bool &diagonal = false,
                                     const bool &dendrogram = false,
                                     const bool &considerWeights=true,
                                     const bool &inverseWeights=false,
                                     const bool &dropIsolates=false);

    void writeClusteringHierarchicalResultsToStream(QTextStream& outText,
                                                    const int N,
                                               const bool &dendrogram = false);

    bool writeCliqueCensus( const QString &fileName,
                            const bool considerWeights);

    void writeClusteringCoefficient(const QString, const bool);

    void writeTriadCensus(const QString, const bool);



    /* DISTANCES, CENTRALITIES & PROMINENCE MEASURES */

    int graphConnectednessFull (const bool updateProgress=false) ;

    bool graphReachable(const int &v1, const int &v2) ;

    void graphMatrixReachabilityCreate() ;

    int graphDiameter(const bool considerWeights, const bool inverseWeights);

    int graphDistanceGeodesic(const int &v1,
                              const int &v2,
                              const bool &considerWeights=false,
                              const bool &inverseWeights=true);

    qreal graphDistanceGeodesicAverage(const bool considerWeights,
                                       const bool inverseWeights,
                                       const bool dropIsolates);

    void graphDistancesGeodesic(const bool &computeCentralities=false,
                                const bool &considerWeights=false,
                                const bool &inverseWeights=true,
                                const bool &dropIsolates=false);

    void graphMatrixDistanceGeodesicCreate(const bool &considerWeights=false,
                                     const bool &inverseWeights=true,
                                     const bool &dropIsolates=false);

    void graphMatrixShortestPathsCreate(const bool &considerWeights=false,
                                        const bool &inverseWeights=true,
                                        const bool &dropIsolates=false) ;

    int getProminenceIndexByName(const QString &prominenceIndexName);

    void prominenceDistribution(const int &index,
                                const ChartType &type,
                                const QString &distImageFileName=QString());

    void prominenceDistributionBars(const H_StrToInt &discreteClasses,
                                    const QString &name,
                                    const QString &distImageFileName);

    void prominenceDistributionArea(const H_StrToInt &discreteClasses,
                                    const QString &name,
                                    const QString &distImageFileName);

    void prominenceDistributionSpline(const H_StrToInt &discreteClasses,
                                      const QString &seriesName,
                                      const QString &distImageFileName);

    void centralityDegree(const bool &weights=true,
                          const bool &dropIsolates=false);

    void centralityInformation(const bool considerWeights=false,
                               const bool inverseWeights=false);

    void centralityEigenvector(const bool &considerWeights=false,
                               const bool &inverseWeights=false,
                               const bool &dropIsolates=false);

    void centralityClosenessIR(const bool considerWeights=false,
                               const bool inverseWeights=false,
                               const bool dropIsolates=false);

    void prestigeDegree(const bool &weights, const bool &dropIsolates=false);

    void prestigePageRank(const bool &dropIsolates=false);

    void prestigeProximity(const bool considerWeights=false,
                           const bool inverseWeights=false,
                           const bool dropIsolates=false);


    /* REACHABILITY AND WALKS */

    int walksBetween(int v1, int v2,int length);

    void graphWalksMatrixCreate(const int &N=0,
                                const int &length=0,
                                const bool &updateProgress=false);

    void writeWalksTotalMatrixPlainText(const QString &fn);

    void writeWalksOfLengthMatrixPlainText(const QString &fn, const int &length);

    void writeMatrixWalks (const QString &fn,
                           const int &length=0,
                           const bool &simpler=false);

    QList<int> vertexinfluenceRange(int v1);

    QList<int> vertexinfluenceDomain(int v2);

    void writeReachabilityMatrixPlainText( const QString &fn,
                                           const bool &dropIsolates=false);


    qreal numberOfTriples(int v1);

    /* CLIQUES, CLUSTERING, TRIADS */
    void graphCliques(QSet<int> R=QSet<int>(), QSet<int> P=QSet<int>(), QSet<int> X=QSet<int>() );

    void graphCliqueAdd (const QList<int> &clique);

    int graphCliquesContaining(const int &actor, const int &size=0);

    int graphCliquesOfSize(const int &size );

    bool graphClusteringHierarchical(Matrix &STR_EQUIV,
                                     const QString &varLocation,
                                     const int &metric,
                                     const int &method,
                                     const bool &diagonal=false,
                                     const bool &diagram=false,
                                     const bool &considerWeights=true,
                                     const bool &inverseWeights=false,
                                     const bool &dropIsolates=false);

    qreal clusteringCoefficientLocal(const int &v1);

    qreal clusteringCoefficient (const bool updateProgress=false);

    bool graphTriadCensus();

    void triadType_examine_MAN_label(int, int, int, GraphVertex*,  GraphVertex*, GraphVertex* );
    //	void eccentr_JordanCenter();    // TODO



    /* LAYOUTS */

    void layoutRandom();

    void layoutRadialRandom(const bool &guides=true);

    void layoutCircular(const double &x0,
                        const double &y0,
                        const double &newRadius,
                        const bool &guides=false);


    void layoutByProminenceIndex ( int prominenceIndex,
                                   int layoutType,
                                   const bool considerWeights=false,
                                   const bool inverseWeights=false,
                                   const bool dropIsolates=false);


    void layoutVertexSizeByIndegree();

    void layoutVertexSizeByOutdegree();


    void layoutForceDirectedSpringEmbedder(const int maxIterations);

    void layoutForceDirectedFruchtermanReingold(const int maxIterations);

    void layoutForceDirectedKamadaKawai(const int maxIterations=500,
                                        const bool considerWeights=false,
                                        const bool inverseWeights=false,
                                        const bool dropIsolates=false,
                                        const QString &initialPositions="current");

    qreal graphDistanceEuclidean(const QPointF &a, const QPointF &b);

    qreal graphDistanceEuclidean(const QPointF &a);

    int sign(const qreal &D);

    qreal layoutForceDirected_F_rep(const QString model,
                                    const qreal &dist,
                                    const qreal &optimalDistance);

    qreal layoutForceDirected_F_att(const QString model,
                                    const qreal &dist,
                                    const qreal &optimalDistance) ;

    void layoutForceDirected_Eades_moveNodes(const qreal &c4);

    void layoutForceDirected_FR_moveNodes(const qreal &temperature) ;

    qreal layoutForceDirected_FR_temperature(const int iteration) const;

    qreal computeOptimalDistance(const int &V);

    void compute_angles( const QPointF &Delta,
                         const qreal &dist,
                         qreal &angle1,
                         qreal &angle2,
                         qreal &degrees1,
                         qreal &degrees2 );



    /* CRAWLER */
    void webCrawlTerminateThreads (QString reason);



    /**RANDOM NETWORKS*/
    void randomizeThings();

    void randomNetErdosCreate (  const int &N,
                                 const QString &model,
                                 const int &m,
                                 const qreal &p,
                                 const QString &mode,
                                 const bool &diag);

    void randomNetScaleFreeCreate (const int &N,
                                    const int &power,
                                    const int &m0,
                                    const int &m,
                                    const qreal &alpha,
                                    const QString &mode);

    void randomNetSmallWorldCreate(const int &N, const int &degree,
                                   const double &beta, const QString &mode);

    void randomNetRingLatticeCreate (const int &N, const int &degree,
                                      const bool updateProgress=false);

    void randomNetRegularCreate (const int &N,
                                    const int &degree,
                                    const QString &mode,
                                    const bool &diag);

    void randomNetLatticeCreate(const int &N,
                                const int &length,
                                const int &dimension,
                                const int &neighborhoodLength,
                                const QString &mode,
                                const bool &circular=false);

    int factorial (int);


    /**  vpos stores the real position of each vertex inside m_graph.
     *  It starts at zero (0).
     *   We need to know the place of a vertex inside m_graph after adding
     *   or removing many vertices
     */
    H_Int vpos;

    // Stores the number of vertices at distance n from a given vertex
    H_f_i sizeOfNthOrderNeighborhood;

    /* maps have O(logN) lookup complexity */
    /* Consider using tr1::hashmap which has O(1) lookup, but this is not ISO C++ yet :(   */


protected: 
    // Called from nodeMovement when a timerEvent occurs
    //void timerEvent(QTimerEvent *event);


private:

    /**
     * List of pointers to the vertices.
     * A vertex stores all the info: links, colours, etc
    */
    VList m_graph;

    GraphicsWidget *m_canvas;

    Parser *file_parser;	//file loader threaded class.

    WebCrawler_Parser *wc_parser;
    WebCrawler_Spider *wc_spider;

    /** private member functions */

    void edgeAdd (const int &v1,
                  const int &v2,
                  const qreal &weight,
                  const int &type,
                  const QString &label,
                  const QString &color
                  );

    /** methods used by graphDistancesGeodesic()  */
    void BFS(const int &s,
             const int &si,
             const bool &computeCentralities=false,
             const bool &dropIsolates=false);

    void dijkstra(const int &s,
                  const int &si,
                  const bool &computeCentralities=false,
                  const bool &inverseWeights=false,
                  const bool &dropIsolates=false);

    void minmax(qreal C,
                GraphVertex *v,
                qreal &max,
                qreal &min,
                int &maxNode, int &minNode
              );

    void resolveClasses ( qreal C,
                          H_StrToInt &discreteClasses,
                          int &classes);

    void resolveClasses ( qreal C,
                           H_StrToInt &discreteClasses,
                           int &classes, int name);


    QList<QString> m_relationsList;

    QList<int> m_graphFileFormatExportSupported;

    QList<int> triadTypeFreqs; 	//stores triad type frequencies

    QList<int> m_verticesList;
    QList<int> m_verticesIsolatedList;

    QList<int> m_verticesSelected;

    QSet<int> m_verticesSet;

    QList<SelectedEdge> m_selectedEdges;

    QHash <int, int> influenceRanges, influenceDomains;

    QHash <int, int> m_vertexPairsNotConnected;
    QHash <int, int> m_vertexPairsUnilaterallyConnected;

    QMap <int, L_int > m_cliques;
    QHash <int, QSet<int> > neighboursHash;

    QList <qreal> m_clusteringLevel;
    QMap <int, V_int> m_clustersPerSequence;

    QMap<QString, V_int> m_clustersByName;
    QMap<int, V_str> m_clusterPairNamesPerSeq;

    Matrix  SIGMA, DM, sumM, invAM, AM, invM, WM;
    Matrix XM, XSM, XRM, CLQM;

    stack<int> Stack;

    /** used in resolveClasses and graphDistancesGeodesic() */
    H_StrToInt discreteDPs, discreteSDCs, discreteCCs, discreteBCs, discreteSCs;
    H_StrToInt discreteIRCCs, discreteECs, discreteEccentricities;
    H_StrToInt discretePCs, discreteICs,  discretePRPs, discretePPs, discreteEVCs;

    QString m_reportsDataDir;
    int m_reportsRealPrecision;
    int m_reportsLabelLength;
    ChartType m_reportsChartType;

    int m_fieldWidth, m_curRelation, m_fileFormat, m_vertexClicked;

    MyEdge m_clickedEdge;

    qreal edgeWeightTemp, edgeReverseWeightTemp;
    qreal meanSDC, varianceSDC;
    qreal meanSCC, varianceSCC;
    qreal meanIRCC, varianceIRCC;
    qreal meanSBC, varianceSBC;
    qreal meanSSC, varianceSSC;
    qreal meanEC, varianceEC;
    qreal meanSPC, varianceSPC;
    qreal meanIC, varianceIC;
    qreal meanEVC, varianceEVC;
    qreal meanSDP, varianceSDP;
    qreal meanPP, variancePP;
    qreal meanPRP, variancePRP;
    qreal minEccentricity, maxEccentricity;
    qreal minSDP, maxSDP, sumDP, sumSDP, groupDP;
    qreal minSDC, maxSDC, sumDC, sumSDC, groupDC;
    qreal minSCC, maxSCC, nomSCC, denomSCC, sumCC, sumSCC, groupCC, maxIndexCC;
    qreal minIRCC, maxIRCC, nomIRCC, denomIRCC, sumIRCC, groupIRCC;
    qreal minSBC, maxSBC, nomSBC, denomSBC, sumBC, sumSBC, groupSBC, maxIndexBC;
    qreal minSPC, maxSPC, nomSPC, denomSPC, t_sumIC, sumSPC, groupSPC, maxIndexPC;
    qreal minSSC, maxSSC, sumSC, sumSSC, groupSC, maxIndexSC;
    qreal minEC, maxEC, nomEC, denomEC, sumEC, groupEC, maxIndexEC;
    qreal minIC, maxIC, nomIC, denomIC, sumIC, maxIndexIC;
    qreal minEVC, maxEVC, nomEVC, denomEVC, sumEVC, sumSEVC, groupEVC;
    qreal minPRP, maxPRP, nomPRC, denomPRC, sumPC, t_sumPRP, sumPRP;
    qreal minPP, maxPP, nomPP, denomPP, sumPP, groupPP;

    qreal minCLC, maxCLC, averageCLC,varianceCLC, d_factor;
    int maxNodeCLC, minNodeCLC;
    int classesSDP, maxNodeDP, minNodeDP;
    int classesSDC, maxNodeSDC, minNodeSDC;
    int classesSCC, maxNodeSCC, minNodeSCC;
    int classesIRCC, maxNodeIRCC, minNodeIRCC;
    int classesSBC, maxNodeSBC, minNodeSBC;
    int classesSPC, maxNodeSPC, minNodeSPC;
    int classesSSC, maxNodeSSC, minNodeSSC;
    int classesEC, maxNodeEC, minNodeEC;
    int classesEccentricity, maxNodeEccentricity, minNodeEccentricity;
    int classesIC, maxNodeIC, minNodeIC;
    int classesPRP, maxNodePRP, minNodePRP;
    int classesPP, maxNodePP, minNodePP;
    int classesEVC, maxNodeEVC, minNodeEVC;
    qreal sizeOfComponent;

    /** General & initialisation variables */

    int m_graphHasChanged;
    int m_totalVertices, m_totalEdges, m_graphDiameter, initVertexSize;
    int initVertexLabelSize, initVertexNumberSize;
    int initVertexNumberDistance, initVertexLabelDistance;
    bool order;
    bool initEdgeWeightNumbers, initEdgeLabels;
    qreal m_graphAverageDistance, m_graphGeodesicsCount;
    qreal m_graphDensity;
    qreal m_graphSumDistance;
    qreal m_graphReciprocityArc, m_graphReciprocityDyad;
    int m_graphReciprocityTiesReciprocated;
    int m_graphReciprocityTiesNonSymmetric;
    int m_graphReciprocityTiesTotal;
    int m_graphReciprocityPairsReciprocated;
    int m_graphReciprocityPairsTotal;

    bool m_graphHasVertexCustomIcons;

    int outboundEdgesVert, inboundEdgesVert, reciprocalEdgesVert;
    //int timerId;
    qreal canvasWidth, canvasHeight;
    bool calculatedEdges;
    bool calculatedVertices, calculatedVerticesList, calculatedVerticesSet;
    bool calculatedAdjacencyMatrix, calculatedDistances, calculatedCentralities;
    bool calculatedIsolates;
    bool calculatedEVC;
    bool calculatedDP, calculatedDC, calculatedPP;
    bool calculatedIRCC, calculatedIC, calculatedPRP;
    bool calculatedTriad;
    bool calculatedGraphSymmetry, calculatedGraphReciprocity;
    bool calculatedGraphDensity, calculatedGraphWeighted;
    bool m_graphIsDirected, m_graphIsSymmetric, m_graphIsWeighted, m_graphIsConnected;

    int cliqueCensusRecursion;

    QString VERSION, fileName, m_graphName, initEdgeColor, initVertexColor,
        initVertexNumberColor, initVertexLabelColor;
    QString initVertexShape, initVertexIconPath;
    QString htmlHead, htmlHeadLight, htmlEnd;

    QDateTime actualDateTime;
};

#endif

