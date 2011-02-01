# format.rb: Written by Tadayoshi Funaba 1999-2009
# $Id: format.rb,v 2.43 2008-01-17 20:16:31+09 tadf Exp $

require 'date' unless defined?(Date::MONTHNAMES)

class Date
  # Holds some constants used by the pure ruby parsing code.
  #
  # The STYLE constant (a hash) allows the user to modify the parsing
  # of DD/DD/DD and DD.DD.DD dates.  For DD/DD/DD dates, you
  # can set the :slash entry to :mdy (month/day/year,
  # :dmy (day/month/year), or :ymd (year/month/day).  The same
  # can be done for DD.DD.DD dates using the :dot entry.  Example:
  #
  #   Date::Format::STYLE[:slash] = :mdy
  #   Date::Format::STYLE[:dot] = :dmy
  module Format
    if RUBY_VERSION < '1.8.7'
      # On Ruby 1.8.6 and earlier, DD/DD/DD and DD.DD.DD dates
      # are interpreted by default as month, day, year.
      STYLE = {:dot=>:mdy, :slash=>:mdy}
    elsif RUBY_VERSION >= '1.9.0'
      # On Ruby 1.9.0 and later , DD/DD/DD and DD.DD.DD dates
      # are interpreted by default as year, month, and day.
      STYLE = {:dot=>:ymd, :slash=>:ymd}
    else
      # On Ruby 1.8.7, DD/DD/DD is interpreted by default as
      # month, day, year, and DD.DD.DD is interpreted
      # by default as year, month, day.
      STYLE = {:dot=>:ymd, :slash=>:mdy}
    end

    # Hash mapping lowercase month names to month numbers (e.g. MONTHS['january'] => 1)
    MONTHS = {
      'january'  => 1, 'february' => 2, 'march'    => 3, 'april'    => 4,
      'may'      => 5, 'june'     => 6, 'july'     => 7, 'august'   => 8,
      'september'=> 9, 'october'  =>10, 'november' =>11, 'december' =>12
    }

    # Hash mapping lowercase day names to day numbers (e.g. DAYS['sunday'] => 0)
    DAYS = {
      'sunday'   => 0, 'monday'   => 1, 'tuesday'  => 2, 'wednesday'=> 3,
      'thursday' => 4, 'friday'   => 5, 'saturday' => 6
    }

    # Hash mapping abbreviated lowercase month names to month numbers (e.g. ABBR_MONTHS['jan'] => 1)
    ABBR_MONTHS = {
      'jan'      => 1, 'feb'      => 2, 'mar'      => 3, 'apr'      => 4,
      'may'      => 5, 'jun'      => 6, 'jul'      => 7, 'aug'      => 8,
      'sep'      => 9, 'oct'      =>10, 'nov'      =>11, 'dec'      =>12
    }

    # Hash mapping abbreviated lowercase day names to day numbers (e.g. ABBR_DAYS['sun'] => 0)
    ABBR_DAYS = {
      'sun'      => 0, 'mon'      => 1, 'tue'      => 2, 'wed'      => 3,
      'thu'      => 4, 'fri'      => 5, 'sat'      => 6
    }
    
    # Hash mapping lowercase time zone names to offsets in seconds (e.g. ZONES['pst'] => -28800)
    ZONES = {
      'ut'  =>  0*3600, 'gmt' =>  0*3600, 'est' => -5*3600, 'edt' => -4*3600,
      'cst' => -6*3600, 'cdt' => -5*3600, 'mst' => -7*3600, 'mdt' => -6*3600,
      'pst' => -8*3600, 'pdt' => -7*3600,
      'a'   =>  1*3600, 'b'   =>  2*3600, 'c'   =>  3*3600, 'd'   =>  4*3600,
      'e'   =>  5*3600, 'f'   =>  6*3600, 'g'   =>  7*3600, 'h'   =>  8*3600,
      'i'   =>  9*3600, 'k'   => 10*3600, 'l'   => 11*3600, 'm'   => 12*3600,
      'n'   => -1*3600, 'o'   => -2*3600, 'p'   => -3*3600, 'q'   => -4*3600,
      'r'   => -5*3600, 's'   => -6*3600, 't'   => -7*3600, 'u'   => -8*3600,
      'v'   => -9*3600, 'w'   =>-10*3600, 'x'   =>-11*3600, 'y'   =>-12*3600,
      'z'   =>  0*3600,

      'utc' =>  0*3600, 'wet' =>  0*3600,
      'at'  => -2*3600, 'brst'=> -2*3600, 'ndt' => -(2*3600+1800),
      'art' => -3*3600, 'adt' => -3*3600, 'brt' => -3*3600, 'clst'=> -3*3600,
      'nst' => -(3*3600+1800),
      'ast' => -4*3600, 'clt' => -4*3600,
      'akdt'=> -8*3600, 'ydt' => -8*3600,
      'akst'=> -9*3600, 'hadt'=> -9*3600, 'hdt' => -9*3600, 'yst' => -9*3600,
      'ahst'=>-10*3600, 'cat' =>-10*3600, 'hast'=>-10*3600, 'hst' =>-10*3600,
      'nt'  =>-11*3600,
      'idlw'=>-12*3600,
      'bst' =>  1*3600, 'cet' =>  1*3600, 'fwt' =>  1*3600, 'met' =>  1*3600,
      'mewt'=>  1*3600, 'mez' =>  1*3600, 'swt' =>  1*3600, 'wat' =>  1*3600,
      'west'=>  1*3600,
      'cest'=>  2*3600, 'eet' =>  2*3600, 'fst' =>  2*3600, 'mest'=>  2*3600,
      'mesz'=>  2*3600, 'sast'=>  2*3600, 'sst' =>  2*3600,
      'bt'  =>  3*3600, 'eat' =>  3*3600, 'eest'=>  3*3600, 'msk' =>  3*3600,
      'msd' =>  4*3600, 'zp4' =>  4*3600,
      'zp5' =>  5*3600, 'ist' =>  (5*3600+1800),
      'zp6' =>  6*3600,
      'wast'=>  7*3600,
      'cct' =>  8*3600, 'sgt' =>  8*3600, 'wadt'=>  8*3600,
      'jst' =>  9*3600, 'kst' =>  9*3600,
      'east'=> 10*3600, 'gst' => 10*3600,
      'eadt'=> 11*3600,
      'idle'=> 12*3600, 'nzst'=> 12*3600, 'nzt' => 12*3600,
      'nzdt'=> 13*3600,

      'afghanistan'           =>   16200, 'alaskan'               =>  -32400,
      'arab'                  =>   10800, 'arabian'               =>   14400,
      'arabic'                =>   10800, 'atlantic'              =>  -14400,
      'aus central'           =>   34200, 'aus eastern'           =>   36000,
      'azores'                =>   -3600, 'canada central'        =>  -21600,
      'cape verde'            =>   -3600, 'caucasus'              =>   14400,
      'cen. australia'        =>   34200, 'central america'       =>  -21600,
      'central asia'          =>   21600, 'central europe'        =>    3600,
      'central european'      =>    3600, 'central pacific'       =>   39600,
      'central'               =>  -21600, 'china'                 =>   28800,
      'dateline'              =>  -43200, 'e. africa'             =>   10800,
      'e. australia'          =>   36000, 'e. europe'             =>    7200,
      'e. south america'      =>  -10800, 'eastern'               =>  -18000,
      'egypt'                 =>    7200, 'ekaterinburg'          =>   18000,
      'fiji'                  =>   43200, 'fle'                   =>    7200,
      'greenland'             =>  -10800, 'greenwich'             =>       0,
      'gtb'                   =>    7200, 'hawaiian'              =>  -36000,
      'india'                 =>   19800, 'iran'                  =>   12600,
      'jerusalem'             =>    7200, 'korea'                 =>   32400,
      'mexico'                =>  -21600, 'mid-atlantic'          =>   -7200,
      'mountain'              =>  -25200, 'myanmar'               =>   23400,
      'n. central asia'       =>   21600, 'nepal'                 =>   20700,
      'new zealand'           =>   43200, 'newfoundland'          =>  -12600,
      'north asia east'       =>   28800, 'north asia'            =>   25200,
      'pacific sa'            =>  -14400, 'pacific'               =>  -28800,
      'romance'               =>    3600, 'russian'               =>   10800,
      'sa eastern'            =>  -10800, 'sa pacific'            =>  -18000,
      'sa western'            =>  -14400, 'samoa'                 =>  -39600,
      'se asia'               =>   25200, 'malay peninsula'       =>   28800,
      'south africa'          =>    7200, 'sri lanka'             =>   21600,
      'taipei'                =>   28800, 'tasmania'              =>   36000,
      'tokyo'                 =>   32400, 'tonga'                 =>   46800,
      'us eastern'            =>  -18000, 'us mountain'           =>  -25200,
      'vladivostok'           =>   36000, 'w. australia'          =>   28800,
      'w. central africa'     =>    3600, 'w. europe'             =>    3600,
      'west asia'             =>   18000, 'west pacific'          =>   36000,
      'yakutsk'               =>   32400
    }

    [MONTHS, DAYS, ABBR_MONTHS, ABBR_DAYS, ZONES].each do |x|
      x.freeze
    end
  end

  # Does various guesses to see which of the middle
  # three arguments is likely to be the day, the month,
  # and the year.  Complex code that doesn't even
  # guess correctly in many cases, but difficult to
  # modify without breaking backwards compatibility.
  def self.s3e(e, y, m, d, bc=false) # :nodoc:
    unless String === m
      m = m.to_s
    end

    if y && m && !d
      y, m, d = d, y, m
    end

    if y == nil
      if d && d.size > 2
        y = d
        d = nil
      end
      if d && d[0,1] == "'"
        y = d
        d = nil
      end
    end

    if y
      y.scan(/(\d+)(.+)?/)
      if $2
        y, d = d, $1
      end
    end

    if m
      if m[0,1] == "'" || m.size > 2
        y, m, d = m, d, y # us -> be
      end
    end

    if d
      if d[0,1] == "'" || d.size > 2
        y, d = d, y
      end
    end

    if y
      y =~ /([-+])?(\d+)/
      if $1 || $2.size > 2
        c = false
      end
      iy = $&.to_i
      if bc
        iy = -iy + 1
      end
      e[:year] = iy
    end

    if m
      m =~ /\d+/
      e[:mon] = $&.to_i
    end

    if d
      d =~ /\d+/
      e[:mday] = $&.to_i
    end

    if c != nil
      e[:_][:comp] = c
    end

  end

  private_class_method :s3e

  # Try to parse the abbreviated day name out of the string. Examples of formats handled: 
  #
  #   sun
  #   MON
  def self._parse_day(str, e) # :nodoc:
    if str.sub!(/\b(#{Format::ABBR_DAYS.keys.join('|')})[^-\d\s]*/io, ' ')
      e[:wday] = Format::ABBR_DAYS[$1.downcase]
      true
    end
  end

  # Try to parse the time including time zone out of the string. Examples of formats handled:
  #
  #   10:20
  #   10:20:30 a.m.
  #   10:20:30.345 pm +10:00
  #   10h 20m pm PDT
  #   10h 20m 30s
  #   10am
  def self._parse_time(str, e) # :nodoc:
    if str.sub!(
                /(
                   (?:
                     \d+\s*:\s*\d+
                     (?:
                       \s*:\s*\d+(?:[,.]\d*)?
                     )?
                   |
                     \d+\s*h(?:\s*\d+m?(?:\s*\d+s?)?)?
                   )
                   (?:
                     \s*
                     [ap](?:m\b|\.m\.)
                   )?
                 |
                   \d+\s*[ap](?:m\b|\.m\.)
                 )
                 (?:
                   \s*
                   (
                     (?:gmt|utc?)?[-+]\d+(?:[,.:]\d+(?::\d+)?)?
                   |
                     [[:alpha:].\s]+(?:standard|daylight)\stime\b
                   |
                     [[:alpha:]]+(?:\sdst)?\b
                   )
                 )?
                /ix,
                ' ')

      t = $1
      e[:zone] = $2 if $2

      t =~ /\A(\d+)h?
              (?:\s*:?\s*(\d+)m?
                (?:
                  \s*:?\s*(\d+)(?:[,.](\d+))?s?
                )?
              )?
            (?:\s*([ap])(?:m\b|\.m\.))?/ix

      e[:hour] = $1.to_i
      e[:min] = $2.to_i if $2
      e[:sec] = $3.to_i if $3
      e[:sec_fraction] = $4.to_i/10.0**$4.size if $4

      if $5
        e[:hour] %= 12
        if $5.downcase == 'p'
          e[:hour] += 12
        end
      end
      true
    end
  end

  # Parse a European date format. Examples of formats handled:
  #
  #   12 Jan 2009
  #   12 Jan bce 2009
  #   2009 Jan 12rd
  def self._parse_eu(str, e) # :nodoc:
    if str.sub!(
                /'?(\d+)[^-\d\s]*
                 \s*
                 (#{Format::ABBR_MONTHS.keys.join('|')})[^-\d\s']*
                 (?:
                   \s*
                   (c(?:e|\.e\.)|b(?:ce|\.c\.e\.)|a(?:d|\.d\.)|b(?:c|\.c\.))?
                   \s*
                   ('?-?\d+(?:(?:st|nd|rd|th)\b)?)
                 )?
                /iox,
                ' ') # '
      s3e(e, $4, Format::ABBR_MONTHS[$2.downcase], $1,
          $3 && $3[0,1].downcase == 'b')
      true
    end
  end

  # Parse a US date format. Examples of formats handled:
  #
  #   Jan 2009 12
  #   Jan 12 2009
  #   Jan 12 bce 2009
  def self._parse_us(str, e) # :nodoc:
    if str.sub!(
                /\b(#{Format::ABBR_MONTHS.keys.join('|')})[^-\d\s']*
                 \s*
                 ('?\d+)[^-\d\s']*
                 (?:
                   \s*
                   (c(?:e|\.e\.)|b(?:ce|\.c\.e\.)|a(?:d|\.d\.)|b(?:c|\.c\.))?
                   \s*
                   ('?-?\d+)
                 )?
                /iox,
                ' ') # '
      s3e(e, $4, Format::ABBR_MONTHS[$1.downcase], $2,
          $3 && $3[0,1].downcase == 'b')
      true
    end
  end

  # Parse an ISO 8601 date format. Examples of formats handled:
  #
  #   2009-01-12
  def self._parse_iso(str, e) # :nodoc:
    if str.sub!(/([-+]?\d+)-(\d+)-(\d+)/, ' ')
      s3e(e, $1, $2, $3, false)
      true
    end
  end

  # Parse some lesser used ISO 8601 date formats (including the commercial
  # week format). Examples of formats handled:
  #
  #   # Commercial week
  #   2009-w01-12
  #   w12-3
  #   # Civil without year
  #   --12-03
  #   --1203
  #   # Ordinal
  #   2009-034
  #   x-034
  def self._parse_iso2(str, e) # :nodoc:
    if str.sub!(/\b(\d{2}|\d{4})?-?w(\d{2})(?:-?(\d))?\b/i, ' ')
      e[:cwyear] = $1.to_i if $1
      e[:cweek] = $2.to_i
      e[:cwday] = $3.to_i if $3
      true
    elsif str.sub!(/-w-(\d)\b/i, ' ')
      e[:cwday] = $1.to_i
      true
    elsif str.sub!(/--(\d{2})?-(\d{2})\b/, ' ')
      e[:mon] = $1.to_i if $1
      e[:mday] = $2.to_i
      true
    elsif str.sub!(/--(\d{2})(\d{2})?\b/, ' ')
      e[:mon] = $1.to_i
      e[:mday] = $2.to_i if $2
      true
    elsif /[,.](\d{2}|\d{4})-\d{3}\b/ !~ str &&
        str.sub!(/\b(\d{2}|\d{4})-(\d{3})\b/, ' ')
      e[:year] = $1.to_i
      e[:yday] = $2.to_i
      true
    elsif /\d-\d{3}\b/ !~ str &&
        str.sub!(/\b-(\d{3})\b/, ' ')
      e[:yday] = $1.to_i
      true
    end
  end

  # Parse the JIS X 0301 date format. Examples of formats handled:
  #
  #   H22.01.12
  def self._parse_jis(str, e) # :nodoc:
    if str.sub!(/\b([mtsh])(\d+)\.(\d+)\.(\d+)/i, ' ')
      era = { 'm'=>1867,
              't'=>1911,
              's'=>1925,
              'h'=>1988
          }[$1.downcase]
      e[:year] = $2.to_i + era
      e[:mon] = $3.to_i
      e[:mday] = $4.to_i
      true
    end
  end

  # Parse some VMS date formats. Examples of formats handled:
  #
  #   2009-jan-12
  #   jan-12-2009
  def self._parse_vms(str, e) # :nodoc:
    if str.sub!(/('?-?\d+)-(#{Format::ABBR_MONTHS.keys.join('|')})[^-]*
                -('?-?\d+)/iox, ' ')
      s3e(e, $3, Format::ABBR_MONTHS[$2.downcase], $1)
      true
    elsif str.sub!(/\b(#{Format::ABBR_MONTHS.keys.join('|')})[^-]*
                -('?-?\d+)(?:-('?-?\d+))?/iox, ' ')
      s3e(e, $3, Format::ABBR_MONTHS[$1.downcase], $2)
      true
    end
  end

  # Parse a slash separated date. Examples of formats handled:
  #
  #   1/2/2009
  #   12/3/07
  def self._parse_sla(str, e) # :nodoc:
    if str.sub!(%r|('?-?\d+)/\s*('?\d+)(?:\D\s*('?-?\d+))?|, ' ') # '
      case Format::STYLE[:slash]
      when :mdy
        s3e(e, $3, $1, $2)
      when :dmy
        s3e(e, $3, $2, $1)
      else
        s3e(e, $1, $2, $3)
      end
      true
    end
  end

  # Parse a period separated date. Examples of formats handled:
  #
  #   1.2.2009
  #   12.3.07
  def self._parse_dot(str, e) # :nodoc:
    if str.sub!(%r|('?-?\d+)\.\s*('?\d+)\.\s*('?-?\d+)|, ' ') # '
      case Format::STYLE[:dot]
      when :mdy
        s3e(e, $3, $1, $2)
      when :dmy
        s3e(e, $3, $2, $1)
      else
        s3e(e, $1, $2, $3)
      end
      true
    end
  end

  # Parse a year preceded by a apostrophe. Examples of formats handled:
  #
  #   '2010
  def self._parse_year(str, e) # :nodoc:
    if str.sub!(/'(\d+)\b/, ' ')
      e[:year] = $1.to_i
      true
    end
  end

  # Parse an abbreviated month name in isolation. Examples of formats handled:
  #
  #   jan
  #   FEB
  def self._parse_mon(str, e) # :nodoc:
    if str.sub!(/\b(#{Format::ABBR_MONTHS.keys.join('|')})\S*/io, ' ')
      e[:mon] = Format::ABBR_MONTHS[$1.downcase]
      true
    end
  end

  # Parse a day of the month. Examples of formats handled:
  #
  #   12th
  #   3rd
  def self._parse_mday(str, e) # :nodoc:
    if str.sub!(/(\d+)(st|nd|rd|th)\b/i, ' ')
      e[:mday] = $1.to_i
      true
    end
  end

  # Parse a completely numeric string of 2-8,10,12,or 14 characters. Examples of formats handled:
  #
  #   12
  #   034
  #   0112
  #   09034
  #   090112
  #   2009034
  #   20090112
  #   2009011210
  #   200901121020
  #   20090112102030
  #   20090112t10
  #   20090112t1020
  #   20090112t102030
  #   102030z
  #   102030-0800
  #   102030+0800
  def self._parse_ddd(str, e) # :nodoc:
    if str.sub!(
                /([-+]?)(\d{2,14})
                  (?:
                    \s*
                    t?
                    \s*
                    (\d{2,6})?(?:[,.](\d*))?
                  )?
                  (?:
                    \s*
                    (
                      z\b
                    |
                      [-+]\d{1,4}\b
                    |
                      \[[-+]?\d[^\]]*\]
                    )
                  )?
                /ix,
                ' ')
      case $2.size
      when 2
        if $3.nil? && $4
          e[:sec]  = $2[-2, 2].to_i
        else
          e[:mday] = $2[ 0, 2].to_i
        end
      when 4
        if $3.nil? && $4
          e[:sec]  = $2[-2, 2].to_i
          e[:min]  = $2[-4, 2].to_i
        else
          e[:mon]  = $2[ 0, 2].to_i
          e[:mday] = $2[ 2, 2].to_i
        end
      when 6
        if $3.nil? && $4
          e[:sec]  = $2[-2, 2].to_i
          e[:min]  = $2[-4, 2].to_i
          e[:hour] = $2[-6, 2].to_i
        else
          e[:year] = ($1 + $2[ 0, 2]).to_i
          e[:mon]  = $2[ 2, 2].to_i
          e[:mday] = $2[ 4, 2].to_i
        end
      when 8, 10, 12, 14
        if $3.nil? && $4
          e[:sec]  = $2[-2, 2].to_i
          e[:min]  = $2[-4, 2].to_i
          e[:hour] = $2[-6, 2].to_i
          e[:mday] = $2[-8, 2].to_i
          if $2.size >= 10
            e[:mon]  = $2[-10, 2].to_i
          end
          if $2.size == 12
            e[:year] = ($1 + $2[-12, 2]).to_i
          end
          if $2.size == 14
            e[:year] = ($1 + $2[-14, 4]).to_i
            e[:_][:comp] = false
          end
        else
          e[:year] = ($1 + $2[ 0, 4]).to_i
          e[:mon]  = $2[ 4, 2].to_i
          e[:mday] = $2[ 6, 2].to_i
          e[:hour] = $2[ 8, 2].to_i if $2.size >= 10
          e[:min]  = $2[10, 2].to_i if $2.size >= 12
          e[:sec]  = $2[12, 2].to_i if $2.size >= 14
          e[:_][:comp] = false
        end
      when 3
        if $3.nil? && $4
          e[:sec]  = $2[-2, 2].to_i
          e[:min]  = $2[-3, 1].to_i
        else
          e[:yday] = $2[ 0, 3].to_i
        end
      when 5
        if $3.nil? && $4
          e[:sec]  = $2[-2, 2].to_i
          e[:min]  = $2[-4, 2].to_i
          e[:hour] = $2[-5, 1].to_i
        else
          e[:year] = ($1 + $2[ 0, 2]).to_i
          e[:yday] = $2[ 2, 3].to_i
        end
      when 7
        if $3.nil? && $4
          e[:sec]  = $2[-2, 2].to_i
          e[:min]  = $2[-4, 2].to_i
          e[:hour] = $2[-6, 2].to_i
          e[:mday] = $2[-7, 1].to_i
        else
          e[:year] = ($1 + $2[ 0, 4]).to_i
          e[:yday] = $2[ 4, 3].to_i
        end
      end
      if $3
        if $4
          case $3.size
          when 2, 4, 6
            e[:sec]  = $3[-2, 2].to_i
            e[:min]  = $3[-4, 2].to_i if $3.size >= 4
            e[:hour] = $3[-6, 2].to_i if $3.size >= 6
          end
        else
          case $3.size
          when 2, 4, 6
            e[:hour] = $3[ 0, 2].to_i
            e[:min]  = $3[ 2, 2].to_i if $3.size >= 4
            e[:sec]  = $3[ 4, 2].to_i if $3.size >= 6
          end
        end
      end
      if $4
        e[:sec_fraction] = $4.to_i/10.0**$4.size
      end
      if $5
        e[:zone] = $5
        if e[:zone][0,1] == '['
          o, n, = e[:zone][1..-2].split(':')
          e[:zone] = n || o
          if /\A\d/ =~ o
            o = format('+%s', o)
          end
          e[:offset] = zone_to_diff(o)
        end
      end
      true
    end
  end

  private_class_method :_parse_day, :_parse_time, 
        :_parse_eu, :_parse_us, :_parse_iso, :_parse_iso2,
        :_parse_jis, :_parse_vms, :_parse_sla, :_parse_dot,
        :_parse_year, :_parse_mon, :_parse_mday, :_parse_ddd

  # call-seq:
  #   _parse(str, comp=true) -> Hash
  #
  # Attempt to parse the string by trying a wide variety of
  # date formats sequentially (unless a match is found by
  # the fast Ragel-based parser).  The +comp+ argument
  # determines whether to convert 2-digit years to 4-digit
  # years. If the +str+ is not in a supported format,
  # an empty hash will be returned.
  #
  # This method searches for a match anywhere in the string,
  # unlike most of the other ruby 1.9-only parsing methods
  # which require that an exact match for the entire string.
  def self._parse(str, comp=true)
    if v = _ragel_parse(str)
      return v
    end

    str = str.dup

    e = {:_ => {:comp => comp}}
    str.gsub!(/[^-+',.\/:@[:alnum:]\[\]]+/, ' ')

    _parse_time(str, e)
    _parse_day(str, e)

    _parse_eu(str, e)     ||
    _parse_us(str, e)     ||
    _parse_iso(str, e)    ||
    _parse_jis(str, e)    ||
    _parse_vms(str, e)    ||
    _parse_sla(str, e)    ||
    _parse_dot(str, e)    ||
    _parse_iso2(str, e)   ||
    _parse_year(str, e)   ||
    _parse_mon(str, e)    ||
    _parse_mday(str, e)   ||
    _parse_ddd(str, e)

    if str.sub!(/\b(bc\b|bce\b|b\.c\.|b\.c\.e\.)/i, ' ')
      if e[:year]
        e[:year] = -e[:year] + 1
      end
    end

    if str.sub!(/\A\s*(\d{1,2})\s*\z/, ' ')
      if e[:hour] && !e[:mday]
        v = $1.to_i
        if (1..31) === v
          e[:mday] = v
        end
      end
      if e[:mday] && !e[:hour]
        v = $1.to_i
        if (0..24) === v
          e[:hour] = v
        end
      end
    end

    if e[:_][:comp]
      if e[:cwyear]
        if e[:cwyear] >= 0 && e[:cwyear] <= 99
          e[:cwyear] += if e[:cwyear] >= 69
                      then 1900 else 2000 end
        end
      end
      if e[:year]
        if e[:year] >= 0 && e[:year] <= 99
          e[:year] += if e[:year] >= 69
                    then 1900 else 2000 end
        end
      end
    end

    e[:offset] ||= zone_to_diff(e[:zone]) if e[:zone]

    e.delete(:_)
    e
  end

if RUBY_VERSION >= '1.9.0'
  # call-seq:
  #   [ruby 1-9 only] <br />
  #   _iso8601(str) -> Hash or nil
  #
  # Attempt to parse string using a wide variety of
  # ISO 8601 based formats, including the civil,
  # commercial, and ordinal formats.
  # Returns a +Hash+ of values if the string was parsed, and +nil+ if not.
  def self._iso8601(str)
    if /\A\s*(([-+]?\d{2,}|-)-\d{2}-\d{2}|
              ([-+]?\d{2,})?-\d{3}|
              (\d{2}|\d{4})?-w\d{2}-\d|
              -w-\d)
        (t
        \d{2}:\d{2}(:\d{2}([,.]\d+)?)?
        (z|[-+]\d{2}(:?\d{2})?)?)?\s*\z/ix =~ str
      _parse(str)
    elsif /\A\s*(([-+]?(\d{2}|\d{4})|--)\d{2}\d{2}|
              ([-+]?(\d{2}|\d{4}))?\d{3}|-\d{3}|
              (\d{2}|\d{4})?w\d{2}\d)
        (t?
        \d{2}\d{2}(\d{2}([,.]\d+)?)?
        (z|[-+]\d{2}(\d{2})?)?)?\s*\z/ix =~ str
      _parse(str)
    elsif /\A\s*(\d{2}:\d{2}(:\d{2}([,.]\d+)?)?
        (z|[-+]\d{2}(:?\d{2})?)?)?\s*\z/ix =~ str
      _parse(str)
    elsif /\A\s*(\d{2}\d{2}(\d{2}([,.]\d+)?)?
        (z|[-+]\d{2}(\d{2})?)?)?\s*\z/ix =~ str
      _parse(str)
    end
  end

  # call-seq:
  #   [ruby 1-9 only] <br />
  #   _rfc3339(str) -> Hash or nil
  #
  # Attempt to parse string using the RFC 3339 format,
  # which is the same as the ISO8601 civil format requiring
  # a time and time zone.
  # Returns a +Hash+ of values if the string was parsed, and +nil+ if not.
  def self._rfc3339(str)
    if /\A\s*-?\d{4}-\d{2}-\d{2} # allow minus, anyway
        (t|\s)
        \d{2}:\d{2}:\d{2}(\.\d+)?
        (z|[-+]\d{2}:\d{2})\s*\z/ix =~ str
      _parse(str)
    end
  end

  # call-seq:
  #   [ruby 1-9 only] <br />
  #   _xmlschema(str) -> Hash or nil
  #
  # Attempt to parse string using the XML schema format,
  # which is similar to the ISO8601 civil format, with
  # most parts being optional.
  # Returns a +Hash+ of values if the string was parsed, and +nil+ if not.
  def self._xmlschema(str)
    if /\A\s*(-?\d{4,})(?:-(\d{2})(?:-(\d{2}))?)?
        (?:t
          (\d{2}):(\d{2}):(\d{2})(?:\.(\d+))?)?
        (z|[-+]\d{2}:\d{2})?\s*\z/ix =~ str
      e = {}
      e[:year] = $1.to_i
      e[:mon] = $2.to_i if $2
      e[:mday] = $3.to_i if $3
      e[:hour] = $4.to_i if $4
      e[:min] = $5.to_i if $5
      e[:sec] = $6.to_i if $6
      e[:sec_fraction] = $7.to_i/10.0**$7.size if $7
      if $8
        e[:zone] = $8
        e[:offset] = zone_to_diff($8)
      end
      e
    elsif /\A\s*(\d{2}):(\d{2}):(\d{2})(?:\.(\d+))?
        (z|[-+]\d{2}:\d{2})?\s*\z/ix =~ str
      e = {}
      e[:hour] = $1.to_i if $1
      e[:min] = $2.to_i if $2
      e[:sec] = $3.to_i if $3
      e[:sec_fraction] = $4.to_i/10.0**$4.size if $4
      if $5
        e[:zone] = $5
        e[:offset] = zone_to_diff($5)
      end
      e
    elsif /\A\s*(?:--(\d{2})(?:-(\d{2}))?|---(\d{2}))
        (z|[-+]\d{2}:\d{2})?\s*\z/ix =~ str
      e = {}
      e[:mon] = $1.to_i if $1
      e[:mday] = $2.to_i if $2
      e[:mday] = $3.to_i if $3
      if $4
        e[:zone] = $4
        e[:offset] = zone_to_diff($4)
      end
      e
    end
  end

  # call-seq:
  #   [ruby 1-9 only] <br />
  #   _rfc2822(str) -> Hash or nil
  #
  # Attempt to parse string using the RFC 2822 format used
  # in email. It's similar to the preferred HTTP format except specific
  # offsets can be used.
  # Returns a +Hash+ of values if the string was parsed, and +nil+ if not.
  def self._rfc2822(str)
    if /\A\s*(?:(?:#{Format::ABBR_DAYS.keys.join('|')})\s*,\s+)?
        \d{1,2}\s+
        (?:#{Format::ABBR_MONTHS.keys.join('|')})\s+
        -?(\d{2,})\s+ # allow minus, anyway
        \d{2}:\d{2}(:\d{2})?\s*
        (?:[-+]\d{4}|ut|gmt|e[sd]t|c[sd]t|m[sd]t|p[sd]t|[a-ik-z])\s*\z/iox =~ str
      e = _parse(str, false)
      if $1.size < 4
        if e[:year] < 50
          e[:year] += 2000
        elsif e[:year] < 1000
          e[:year] += 1900
        end
      end
      e
    end
  end

  class << self;
    # [ruby 1.9 only]
    alias_method :_rfc822, :_rfc2822
  end

  # call-seq:
  #   [ruby 1-9 only] <br />
  #   _httpdate(str) -> Hash or nil
  #
  # Attempt to parse string using the 3 HTTP formats specified
  # in RFC 2616.
  # Returns a +Hash+ of values if the string was parsed, and +nil+ if not.
  def self._httpdate(str)
    if /\A\s*(#{Format::ABBR_DAYS.keys.join('|')})\s*,\s+
        \d{2}\s+
        (#{Format::ABBR_MONTHS.keys.join('|')})\s+
        -?\d{4}\s+ # allow minus, anyway
        \d{2}:\d{2}:\d{2}\s+
        gmt\s*\z/iox =~ str
      _rfc2822(str)
    elsif /\A\s*(#{Format::DAYS.keys.join('|')})\s*,\s+
        \d{2}\s*-\s*
        (#{Format::ABBR_MONTHS.keys.join('|')})\s*-\s*
        \d{2}\s+
        \d{2}:\d{2}:\d{2}\s+
        gmt\s*\z/iox =~ str
      _parse(str)
    elsif /\A\s*(#{Format::ABBR_DAYS.keys.join('|')})\s+
        (#{Format::ABBR_MONTHS.keys.join('|')})\s+
        \d{1,2}\s+
        \d{2}:\d{2}:\d{2}\s+
        \d{4}\s*\z/iox =~ str
      _parse(str)
    end
  end

  # call-seq:
  #   [ruby 1-9 only] <br />
  #   _jisx0301(str) -> Hash or nil
  #
  # Attempt to parse string using the JIS X 0301 date format or
  # ISO8601 format.
  # Returns a +Hash+ of values if the string was parsed, and +nil+ if not.
  def self._jisx0301(str)
    if /\A\s*[mtsh]?\d{2}\.\d{2}\.\d{2}
        (t
        (\d{2}:\d{2}(:\d{2}([,.]\d*)?)?
        (z|[-+]\d{2}(:?\d{2})?)?)?)?\s*\z/ix =~ str
      if /\A\s*\d/ =~ str
        _parse(str.sub(/\A\s*(\d)/, 'h\1'))
      else
        _parse(str)
      end
    else
      _iso8601(str)
    end
  end
end
end
