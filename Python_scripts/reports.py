#!/usr/bin/env python3

from reportlab.platypus import SimpleDocTemplate
from reportlab.platypus import Paragraph, Spacer, Table, Image
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.lib import colors
from reportlab.graphics.shapes import Drawing
from reportlab.graphics.charts.piecharts import Pie
from reportlab.graphics.charts.barcharts import HorizontalBarChart
from reportlab.lib.units import inch, cm
from reportlab.graphics.charts.legends import Legend
from reportlab.lib.colors import black, red, purple, green, maroon, brown, pink, white, HexColor
from reportlab.lib.validators import Auto
import locale

def generate(filename, title, additional_info, table_data):
  styles = getSampleStyleSheet()
  report = SimpleDocTemplate(filename)
  report_title = Paragraph(title, styles["h1"])
  report_info = Paragraph(additional_info, styles["BodyText"])
  table_style = [('GRID', (0,0), (-1,-1), 1, colors.black),
                ('FONTNAME', (0,0), (-1,0), 'Helvetica-Bold'),
                ('ALIGN', (0,0), (-1,-1), 'CENTER')]
  report_table = Table(data=table_data, style=table_style, hAlign="LEFT")
  empty_line = Spacer(1,20)
  report_chart = generate_chart(table_data)
  report.build([report_title, empty_line, report_info, empty_line, report_chart, empty_line, report_table])


def generate_chart(table_data):
  report_pie = Pie()
  report_pie.data = []
  report_pie.labels = []
  report_pie.sideLabels = True
  report_pie.simpleLabels = False
  report_pie.checkLabelOverlap = True
  report_pie.slices.labelRadius = 1.75
  report_pie.slices.fontSize = 10
  report_pie.slices.popout = 5
  report_pie.width = 100
  report_pie.height = 100
  report_pie.x = 100
  report_pie.y = 35
  
  
  pie_legend = Legend()
  pie_legend.x               = 230
  pie_legend.y               = 85
  pie_legend.dx              = 8
  pie_legend.dy              = 8
  pie_legend.fontName        = 'Helvetica'
  pie_legend.fontSize        = 9
  pie_legend.boxAnchor       = 'w'
  pie_legend.columnMaximum   = 10
  pie_legend.strokeWidth     = 1
  pie_legend.strokeColor     = black
  pie_legend.deltax          = 75
  pie_legend.deltay          = 10
  pie_legend.autoXPadding    = 5
  pie_legend.yGap            = 0
  pie_legend.dxTextSpace     = 5
  pie_legend.alignment       = 'right'
  pie_legend.dividerLines    = 1|2|4
  pie_legend.dividerOffsY    = 4.5
  pie_legend.subCols.rpad    = 30
  pie_legend.colorNamePairs = Auto(obj=report_pie)

  for car in table_data[1:11]:
    report_pie.data.append(car[3])
    report_pie.labels.append(car[1])

  # report_bar = HorizontalBarChart()
  data = []
  names = []
  for car in table_data[1:11]:
    data.append((car[3]*locale.atof(car[2].strip("$")))/100000)
    names.append(car[1])

  print([tuple(data)])
  # #data = [
  #          (13, 5, 20, 22, 37, 98, 19, 4),
  #          ]

  #names = ["Cat %s" % i for i in range(1, len(data[0])+1)]

  report_bar = HorizontalBarChart()
  report_bar.x = 40
  report_bar.y = 10
  report_bar.height = 200
  report_bar.width = 370
  report_bar.data = [tuple(data)]
  report_bar.strokeColor = colors.white
  report_bar.valueAxis.valueMin = 0
  report_bar.valueAxis.valueMax = 240
  report_bar.valueAxis.valueStep = 30
  report_bar.categoryAxis.labels.boxAnchor = 'ne'
  report_bar.categoryAxis.labels.dx = -10
  report_bar.categoryAxis.labels.fontName = 'Helvetica'
  report_bar.categoryAxis.categoryNames = names
  
  report_chart = Drawing()
  report_chart.add(report_pie)
  report_chart.add(report_bar)
  return report_chart

def generate_bar_chart():
  pass